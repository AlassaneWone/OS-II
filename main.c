#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>
#include "const.h"
#include "course.h"
#include "voiture.h"
#include "affiche.h"

int PILOTES[20];   
int TEMPSMAXCOURSE = 0;
int NBRVOITCOURSE = 20;
int TOURMAXCOURSE = TOURDEFINAL;

int quelCourse(char *argv[]) {
	// détermine le tpye de la course en fonction de l'agument donné par l'utilisateur ( de 1(=P1) à 7(=Final))
	int type = 0;
  	if (strcmp(argv[1], "P1") == 0) {
  		TEMPSMAXCOURSE = TEMPS_P;
    	type = 1;
  	} else if (strcmp(argv[1], "P2") == 0) {
  		TEMPSMAXCOURSE = TEMPS_P;
    	type = 2;
  	} else if (strcmp(argv[1], "P3") == 0) {
  		TEMPSMAXCOURSE = TEMPS_P;
    	type = 3;
  	} else {
    	switch (argv[1][0]) {
      		case 'Q':
        		TEMPSMAXCOURSE = TEMPS_Q1;
        		type = 4;
        		if (argv[1][1] == '2') {
          			NBRVOITCOURSE = 15;
          			TEMPSMAXCOURSE = TEMPS_Q2;
          			type = 5;
        		} else if (argv[1][1] == '3') {
          			NBRVOITCOURSE = 10;
          			TEMPSMAXCOURSE = TEMPS_Q3;
          			type = 6;
        		}
        		break;
      		case 'F':
        		type = 7;
        		break;
        	case 'S':
        		type = 8;
        		TOURMAXCOURSE = 14;
    	}
  	}
  	return type;
}

void initPilote(int typeCourse){
	// Détermine les pilotes de la courses et on les mets dans la const global
	int file;
	int piloteClasse[20];
    char *listePilote = malloc(sizeof(char)*200); //Pour garder en mémoire les numéros de pilote
    char *token;

    int pilotePrimaire[NBRVOITURE] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9 };
	int l =0;
    if(typeCourse == 5 || typeCourse == 6 ){ 
    	// si course avec des pilotes de départ spécial

        if(typeCourse == 5){
            file = open("classement/Q1",O_RDONLY);
        }
        else if(typeCourse == 6){
            file = open("classement/Q2",O_RDONLY);
        }
        // on lit le classement
        read(file,listePilote,sizeof(listePilote)*256); 
        
        close(file);

        token = strtok(listePilote, "\n");

        while( token != NULL ) { 
        	// on parcours la liste ligne par ligne
            piloteClasse[l] = atoi(token); 
            // atoi convetit la char en int
            token = strtok(NULL, "\n");
            l++;
        }

        for(int i=0;i<NBRVOITCOURSE;i++){
        	// on met les numéros de pilote dans l'ordre
            PILOTES[i] = piloteClasse[i]; 
        }
        
        
    }else{
        for(int n=0;n<NBRVOITCOURSE;n++){
            PILOTES[n] = pilotePrimaire[n];
        }
    }

}

int main(int argc, char *argv[]){
	//Gestion de la course
	if (argc != 2){
        printf("1 seul paramètre \n");
        exit(0);
    }
    int typeCourse = quelCourse(argv);
    if (typeCourse == 0){
        printf("Erreur de paramètre\n");
        exit(0);
    }
    Voiture *voit ;

    // Mémoire partagée
    int shmid = shmget(IPC_PRIVATE, sizeof(Voiture) * NBRVOITURE, 0666 | IPC_CREAT);
    voit = shmat(shmid, 0, 0);
    // Sémaphore
    int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
    sem_t *semaphore = shmat(shmid_sem, 0, 0);
    sem_init(semaphore, 1, 1);

    pid_t pid;
    initPilote(typeCourse);  //on récupère les pilotes

    int i = 0;
    for (i; i < NBRVOITCOURSE; i++) {
    	pid = fork();
        if (pid == 0){
        	break;
        }
    }
    if (pid == -1) {
    	perror("Erreur fork ");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { 
    	//Fils
    	//On lance les courses
    	if (typeCourse < 7) {
    		course(&voit[i], PILOTES[i], TEMPSMAXCOURSE, semaphore);
		}else if (typeCourse >=7) {
            coursefinal(&voit[i], PILOTES[i], semaphore, TOURMAXCOURSE);
        }
        exit(0);
	}

    //Père
    // Affiche les résultas à l'écran
    else {
    	bool is_course_finie = false;
        int nmbr_voiture_finie = 0;
            
        while (!is_course_finie) {
			
		  	for (int j = 0; j < NBRVOITCOURSE; j++) {
		  		//Compte le nombre de voiture qui ont finie
				if ((typeCourse < 7 && voit[j].tempsTotal > 3600000) || (typeCourse >= 7 && voit[j].tour >= TOURMAXCOURSE) || voit[j].etat == 2) {
					//Compte les tours si Final, Compte le temps pour les autres
			  		nmbr_voiture_finie++;
				}
				if (nmbr_voiture_finie == NBRVOITCOURSE +1) {
			  		is_course_finie = true;
				}
		  	}
		  	//Affiches les temps
		  	if (typeCourse < 7){
		  		affiche(voit, semaphore, NBRVOITCOURSE);
		  	}else if (typeCourse >= 7){
		  		afficheFinal(voit, semaphore);
		  	}
		  	
		  	sleep(1); // Délai d'affichage, sinon illisble
		}


        sauvegardeClassement(voit, typeCourse, NBRVOITCOURSE); // On sauvegarde les classements dans le dossier classement
        
        //Supprime sémaphore, mémoire patagée
        shmdt(voit); 
        shmctl(shmid, IPC_RMID, NULL); 
        sem_destroy(semaphore);
        exit(0);
	}
}

