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
int TEMPSMAXCOURSE = TEMPS_P;
int NBRVOITCOURSE = 20;

void initPilote(Voiture *vdata, int typeCourse){
	int file;
	int piloteClasse[20];
    char *listePilote = malloc(sizeof(char)*200); //Pour garder en mémoire les numéros de pilote
    char *token;

    int pilotePrimaire[NBRVOITURE] = {44,77,11,33,3,4,5,18,14,31,16,55,10,22,7,99,9,47,6,63};  //Numéros des voitures

	int l =0;
    if(typeCourse == 5 || typeCourse == 6 ){ // si course avec des pilotes de départ spécial

        if(typeCourse == 5){
            file = open("classement/Q1",O_RDONLY);
        }
        else if(typeCourse == 6){
            file = open("classement/Q2",O_RDONLY);
        }
        read(file,listePilote,sizeof(listePilote)*256); // on lit le classement
        close(file);

        token = strtok(listePilote, "\n");

        while( token != NULL ) { // on parcours la liste ligne par ligne
            piloteClasse[l] = atoi(token); // atoi convetit la char en int
            token = strtok(NULL, "\n");
            l++;
        }

        for(int i=0;i<NBRVOITCOURSE;i++){
            PILOTES[i] = piloteClasse[i]; // on met les numéros de pilote dans l'ordre
        }
    }else{
        for(int n=0;n<NBRVOITCOURSE;n++){
            PILOTES[n] = pilotePrimaire[n];
        }
    }

}
int quelCourse(char *argv[]) {

    if(strcmp(argv[1], "P1" ) == 0){
        return 1;

    }
    else if(strcmp(argv[1], "P2") == 0) {
        return 2;

    }
    else if(strcmp(argv[1], "P3") == 0) {
        return 3;

    }
    else if(strcmp(argv[1], "Q1") == 0) {
        TEMPSMAXCOURSE = TEMPS_Q1;
        return 4;

    }
    else if(strcmp(argv[1], "Q2") == 0) {
        NBRVOITCOURSE = 15;
        TEMPSMAXCOURSE = TEMPS_Q2;
        return 5;

    }
    else if(strcmp(argv[1], "Q3")  == 0) {
        NBRVOITCOURSE = 10;
        TEMPSMAXCOURSE = TEMPS_Q3;
        return 6;

    }
    else if(strcmp(argv[1], "F")  == 0) {
        return 7;

    }
    return 0;
}

int main(int argc, char *argv[]){
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

    // mémoire partagée
    int shmid = shmget(IPC_PRIVATE, sizeof(Voiture) * NBRVOITURE, 0666 | IPC_CREAT);
    voit = shmat(shmid, 0, 0);

    //création et initiation du sémaphore
    int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
    sem_t *semaphore = shmat(shmid_sem, 0, 0);
    sem_init(semaphore, 1, 1);


    int i = 0;
    pid_t pid;
    initPilote(voit, typeCourse);


    //Processus Fils
    for (i; i < NBRVOITCOURSE; i++) {
    	pid = fork();
        if (pid == 0) break;
    }
    if (pid == -1) {
    	perror("Erreur fork ");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
    	if (typeCourse < 7) {
    		course(&voit[i], PILOTES[i], TEMPSMAXCOURSE, semaphore);
		}else {
            coursefinal(&voit[i], PILOTES[i], semaphore);
        }
        exit(0);
	}

    //Proccessus Père
    else {
    	bool is_course_finie = false;
        int nmbr_voiture_finie = 0;
            
        while (!is_course_finie) {
        	if (typeCourse <7){
            	affiche(voit, semaphore, NBRVOITCOURSE);
                    
                for (int j = 0; j < NBRVOITCOURSE; j++) {
                	// compte le nombre de voiture qui ont finit les essais
                    if (voit[j].tempTotal > TEMPSMAXCOURSE || voit[j].etat == 2) {
                    	nmbr_voiture_finie++;
                    }
                    if (nmbr_voiture_finie == NBRVOITCOURSE) {
                    	// Toutes les voitures ont finies, fin de la course
                        is_course_finie = true;
                    } 
                }
                sleep(1);
            }else if (typeCourse == 7){ 
            	afficheFinal(voit, semaphore);
                nmbr_voiture_finie = 0;
                for (int j = 0; j < NBRVOITCOURSE; j++) {
                	// compte le nombre de voiture qui ont finit la course
                    if (voit[j].tour >= TOURDEFINAL || voit[j].etat == 2) {
                    nmbr_voiture_finie++;
                	}
                	if (nmbr_voiture_finie == NBRVOITCOURSE) {
                		// Toutes les voitures ont finies, fin de la course
                    	is_course_finie = true;
                	} 
             	}
                sleep(1);
            }
                    
		}
        sauvegardeClassement(voit, typeCourse, NBRVOITCOURSE);
            
        shmdt(voit); 
        shmctl(shmid, IPC_RMID, NULL); 
        sem_destroy(semaphore);
        exit(0);
	}
}

