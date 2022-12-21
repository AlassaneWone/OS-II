
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "const.h"
#include "voiture.h"

Voiture copieVoit[NBRVOITURE];

int fctTri(const void *a, const void *b){
	//Fonction qui permet de trier, avec qsort, les voitures en fonction de leur meilleur tour 
    Voiture *voit1 = (Voiture *) a;
    Voiture *voitSuivante = (Voiture *) b;

    if(voit1->meilleurTour > voitSuivante->meilleurTour){
        return 1;
    } else if(voit1->meilleurTour < voitSuivante->meilleurTour){
        return -1;
    } else {
        return 0;
    }
}

int diffTemps(int i){
	// Permet de déterminer la différence de temps des meilleurs tours entre une voiture et sa précédente dans la liste
    if (i != 0 && copieVoit[i].meilleurTour != 0){
        return (copieVoit[i].meilleurTour - copieVoit[i-1].meilleurTour);
    }else
    {
        return 0;
    }
}

int diffTempsFinal(int i){
	// Permet de déterminer la différence de temps entre une voiture et sa précédente dans la liste
    if(copieVoit[i].out==1){
        return 0;
    }
    else if (i > 0){
        return (copieVoit[i].tempTotal - copieVoit[i-1].tempTotal);
    }else{
        return 0;
    }
}

int quiMeilleurSecteur(int secteur){
	// Trouve l'index de la voiture qui a le meilleur temps
    int indexMeilleurVoit = 0;
    int meilleurTemps = 0;
	int indexSecteur = secteur - 1;
    for(int i=0;i<NBRVOITURE;i++){
            if( copieVoit[i].meilleurSecteur[indexSecteur] != 0 &&(copieVoit[i].meilleurSecteur[indexSecteur] < meilleurTemps || meilleurTemps == 0 )){
                meilleurTemps = copieVoit[i].meilleurSecteur[indexSecteur];
                indexMeilleurVoit = i;
            }
    }
    return indexMeilleurVoit;
}

int quiMeilleurTour(){
	// Trouve l'index de la voiture qui a le meilleur tour
    int index = 0;
    int best = 0;

    for(int i=0; i < NBRVOITURE; i++){
        if( copieVoit[i].meilleurTour != 0 &&(copieVoit[i].meilleurTour < best || best == 0)){
            best = copieVoit[i].meilleurTour;
            index = i;
        }
    }

    return index;
}



void affiche(Voiture *voitPartage,sem_t *semaphore, int NBRVOITCOURSE){
	// Affiche le tableau, trie, à l'écran. Avec toutes les donnees
    float dif;
    
    char *pit;
    char *out;

    sem_wait(semaphore);
	// On va recuperer les données et les copiees
    for (int i=0; i<NBRVOITCOURSE;i++) {
        copieVoit[i] = voitPartage[i];
    }
    sem_post(semaphore);

    qsort(copieVoit, NBRVOITCOURSE, sizeof(Voiture), fctTri); // Trie les voitures
    
	system("clear"); // Vide la console
    printf("\n");
    // Affiche les titres
    printf(" %-*s  %*s  %*s  %*s  %*s  %*s  %*s  %*s  %*s  %*s \n",10,"Classement",10,"Pilote",10,"S1",10,"S2",10,"S3",10,"Meilleur Tour",10,"TempsDif",5,"Tours",5,"PIT",5,"OUT");
    printf("\n");


    for(int i=0; i<NBRVOITCOURSE; i++){
        dif = diffTemps(i);
        // Détermine si la voiture est en pit ou out
        if(copieVoit[i].pit==1){
            pit="PIT";
        }else{
            pit=" ";
        }
        if(copieVoit[i].out==1){
            out="OUT";
        }else{
            out=" ";
        }
		// Affiches les données
    	printf(" %-*d  %*d  %*.3f  %*.3f  %*.3f  %*.3f  %*.3f  %*d  %*s  %*s \n",
                   10,i+1,10,copieVoit[i].pilote,10,copieVoit[i].secteur[0]/1000,10, copieVoit[i].secteur[1]/1000,10,copieVoit[i].secteur[2]/1000,10,copieVoit[i].meilleurTour/1000,10,dif/1000,5,copieVoit[i].tour,8, pit,5, out);
        

    }
	// Affiche les meilleurs temps S1,S2,S3 et Tour
    printf("\n Meilleur tour     Pilote %d\t %.3f   \n",copieVoit[quiMeilleurTour()].pilote, copieVoit[quiMeilleurTour()].meilleurTour/1000);

    printf("\n Meilleur S1  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(1)].pilote, copieVoit[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
    printf(" Meilleur S2  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(2)].pilote, copieVoit[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
    printf(" Meilleur S3  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(3)].pilote, copieVoit[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    printf("\n");

}
int fctTriFinal(const void *a, const void *b){
	//Fonction qui permet de trier, avec qsort, les voitures en fonction de leur Temps en course (ou si elles sont out)
    Voiture *voit1 = (Voiture *) a;
    Voiture *voitSuivante = (Voiture *) b;


	if(voit1->etat==2 && voit1->tour < voitSuivante->tour){
		// Si voit1 est out ET si voit1 est derrière voitSuivante
        return 1;
    }else if(voitSuivante->etat==2 || voit1->tempTotal < voitSuivante->tempTotal){
    	// Si voitSuivante est out OU voit1 derrière voitSuuivante
        return -1;
    }else if(voit1->tempTotal > voitSuivante->tempTotal && voit1->etat!=2){
    	// Si voit1 devant voitSuivante ET voit1 n'est pas out
        return 1;
    }else{
        return 0;
    }

}
void afficheFinal(Voiture *voitPartage,sem_t *semaphore){
	// Affiche le tableau, trie, a l'écran. Avec toutes les donnees
    float dif;
    char *pit;
    char *out;
    system("clear"); // vide la console

    sem_wait(semaphore);
    // Accède au données
    for (int i=0; i<NBRVOITURE;i++) {
        copieVoit[i] = voitPartage[i];
    }
    sem_post(semaphore);

    qsort(copieVoit, NBRVOITURE, sizeof(Voiture), fctTriFinal); // Trie les voitures

    printf("\n");
    printf(" %-*s  %*s  %*s  %*s  %*s  %*s  %*s  %*s  %*s \n",10,"Classement",10,"Pilote",10,"S1",10,"S2",10,"S3",10,"TempsDif",5,"Tours",5,"P",5,"O");


    for(int i=0; i<NBRVOITURE; i++){
        dif = diffTempsFinal(i);
		// Détermine si la voiture est en pit ou out
        if(copieVoit[i].pit==1){
            pit="PIT";
        }else{
            pit=" ";
        }
        if(copieVoit[i].out==1){
            out="OUT";
        }else{
            out=" ";
        }
		// Affiche les données
        printf(" %-*d  %*d  %*.3f  %*.3f  %*.3f  %*.3f  %*d  %*s  %*s \n",
                   10,i+1,10,copieVoit[i].pilote,10,copieVoit[i].secteur[0]/1000,10, copieVoit[i].secteur[1]/1000,10,copieVoit[i].secteur[2]/1000,10,dif/1000,5,copieVoit[i].tour,5, pit,5, out);

    }
	// Affiche les meilleurs temps S1,S2,S3 et Tour
    printf("\n Meilleur tour \t   Pilote %d\t %.3f   \n",copieVoit[quiMeilleurTour()].pilote, copieVoit[quiMeilleurTour()].meilleurTour/1000);
    printf("\n Meilleur S1  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(1)].pilote, copieVoit[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
    printf(" Meilleur S2  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(2)].pilote, copieVoit[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
    printf(" Meilleur S3  \t   Pilote %d\t %.3f   \n", copieVoit[quiMeilleurSecteur(3)].pilote, copieVoit[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    printf("\n");

}


void sauvegardeClassement(Voiture *voitPartage, int typeCourse,int NBRVOITCOURSE) {
	// Ouvre le fichier en fonction du type de course et y écris les données
    int file;
    for (int i=0; i<NBRVOITCOURSE;i++) {
        copieVoit[i] = voitPartage[i];
    }
	switch (typeCourse){
		case 1:
			file = open("classement/P1",O_WRONLY);
			break;
		case 2:
			file = open("classement/P2",O_WRONLY);
			break;
		case 3:
			file = open("classement/P3",O_WRONLY);
			break;
		case 4:
			file = open("classement/Q1",O_WRONLY);
			break;
		case 5:
			file = open("classement/Q2",O_WRONLY);
			break;
		case 6:
			file = open("classement/Q3",O_WRONLY);
			break;
		case 7:
			file = open("classement/Final",O_WRONLY);
			if (file<0) {
            	perror("FILE ERROR ");
            	return;
        	}
        	char buffer[2048];

        	qsort(copieVoit, NBRVOITCOURSE, sizeof(Voiture), fctTriFinal);

        	for(int i =0 ; i<NBRVOITCOURSE; i++){
            	sprintf(buffer,"N°%d \t% d \n",i+1,copieVoit[i].pilote);
            	write(file,buffer, strlen(buffer));
        	}

        	sprintf(buffer,"Meilleur tour \t Voiture %d \t [%.3f] \n",copieVoit[quiMeilleurTour()].pilote, copieVoit[quiMeilleurTour()].meilleurTour/1000);
			write(file,buffer, strlen(buffer));
			sprintf(buffer,"Best S1 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(1)].pilote, copieVoit[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
			write(file,buffer, strlen(buffer));
			sprintf(buffer,"Best S2 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(2)].pilote, copieVoit[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
			write(file,buffer, strlen(buffer));
    		sprintf(buffer,"Best S3 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(3)].pilote, copieVoit[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    		write(file,buffer, strlen(buffer));
    		close(file);
			break;
	}

    char buff[2048];

    qsort(copieVoit, NBRVOITCOURSE, sizeof(Voiture), fctTri);

    for(int i =0 ; i<NBRVOITCOURSE; i++){
        sprintf(buff,"%d\n",copieVoit[i].pilote);
        write(file,buff, strlen(buff));
    }
	
	sprintf(buff,"Meilleur tour \t Voiture %d \t [%.3f] \n",copieVoit[quiMeilleurTour()].pilote, copieVoit[quiMeilleurTour()].meilleurTour/1000);
	write(file,buff, strlen(buff));
	sprintf(buff,"Best S2 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(1)].pilote, copieVoit[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
	write(file,buff, strlen(buff));
	sprintf(buff,"Best S2 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(2)].pilote, copieVoit[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
	write(file,buff, strlen(buff));
    sprintf(buff,"Best S3 \t Voiture %d \t [%.3f] \n", copieVoit[quiMeilleurSecteur(3)].pilote, copieVoit[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    write(file,buff, strlen(buff));
    close(file);
}




