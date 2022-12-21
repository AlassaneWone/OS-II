
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "const.h"
#include "voiture.h"

Voiture copyCar[NBRVOITURE];


int quiMeilleurSecteur(int secteur){
    int indexMeilleurVoit = 0;
    int meilleurTemps = 0;
	int indexSecteur = secteur - 1;
    for(int i=0;i<NBRVOITURE;i++){
            if( copyCar[i].meilleurSecteur[indexSecteur] != 0 &&(copyCar[i].meilleurSecteur[indexSecteur] < meilleurTemps || meilleurTemps == 0 )){
                meilleurTemps = copyCar[i].meilleurSecteur[indexSecteur];
                indexMeilleurVoit = i;
            }
    }
    return indexMeilleurVoit;
}

int quiMeilleurLap(){
    int index = 0;
    int best = 0;

    for(int i=0; i < NBRVOITURE; i++){
        if( copyCar[i].meilleurTour != 0 &&(copyCar[i].meilleurTour < best || best == 0)){
            best = copyCar[i].meilleurTour;
            index = i;
        }
    }

    return index;
}

int fctTri(const void *a, const void *b){
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
    if (i != 0 && copyCar[i].meilleurTour != 0){
        return -(copyCar[i-1].meilleurTour - copyCar[i].meilleurTour);
    }else
    {
        return 0;
    }
}

int diffTempsFinal(int i){
    if(copyCar[i].out==1){
        return 0;
    }
    else if (i != 0 && copyCar[i].meilleurTour != 0){
        return -(copyCar[i-1].tempTotal - copyCar[i].tempTotal);
    }else
    {
        return 0;
    }
}

void affiche(Voiture *vdata,sem_t *semaphore, int NBRVOITCOURSE){
    float dif;
    
    char *pit;
    char *out;

    sem_wait(semaphore);

    for (int i=0; i<NBRVOITCOURSE;i++) {
        copyCar[i] = vdata[i];
    }
    sem_post(semaphore);

    qsort(copyCar, NBRVOITCOURSE, sizeof(Voiture), fctTri);
    
	system("clear");
    printf("\n");
    printf("| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\n",5,"Pos.",10,"Numero",10,"Secteur 1",10,"Secteur 2",10,"Secteur 3",10,"meilleur Tour",10,"TempsDif",6,"Tours",3,"PIT",3,"OUT");
    printf("\n");


    for(int i=0; i<NBRVOITCOURSE; i++){
        dif = diffTemps(i);
        if(copyCar[i].pit==1){
            pit="PIT";
        }else{
            pit=" ";
        }
        if(copyCar[i].out==1){
            out="OUT";
        }else{
            out=" ";
        }

    	printf("| %*d | %*d | %*.3f | %*.3f | %*.3f | %*.3f | %*.3f | %*d | %*s | %*s |\n",
                   5,i+1,10,copyCar[i].pilote,10,(float)copyCar[i].secteur[0]/1000,10, (float)copyCar[i].secteur[1]/1000,10,(float)copyCar[i].secteur[2]/1000,10,(float)copyCar[i].meilleurTour/1000,10,dif/1000,6,copyCar[i].tour,3, pit,3, out);
        

    }


    printf("\n| Meilleur tour |   Voiture %d\t [%.3f]   |\n",copyCar[quiMeilleurLap()].pilote, (float)copyCar[quiMeilleurLap()].meilleurTour/1000);

    printf("\n| Best S1  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(1)].pilote, (float)copyCar[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
    printf("| Best S2  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(2)].pilote, (float)copyCar[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
    printf("| Best S3  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(3)].pilote, (float)copyCar[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    printf("\n");

}
int fctTriFinal(const void *a, const void *b){
    const Voiture *p1 = (Voiture *) a;
    const Voiture *p2 = (Voiture *) b;

    if(p1->etat==2 && p1->tour < p2->tour){
        return 1;
    }
    else if(p2->etat==2){
        return -1;
    }
    else if(p1->tempTotal > p2->tempTotal && p1->etat!=2){
        return 1;
    }
    else if(p1->tempTotal < p2->tempTotal && p2->etat!=2){
        return -1;
    }
    else{
        return 0;
    }

}
void afficheFinal(Voiture *vdata,sem_t *semaphore){
    float dif;
    char *pit;
    char *out;
    system("clear");

    sem_wait(semaphore);
    for (int i=0; i<NBRVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    sem_post(semaphore);

    qsort(copyCar, NBRVOITURE, sizeof(Voiture), fctTriFinal);

    ///CREATION DU TABLEAU
    for(int i=0;i<95;i++){
        printf("_");
    }
    printf("\n");
    printf("| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\n",5,"Pos.",10,"Numero",10,"Secteur 1",10,"Secteur 2",10,"Secteur 3",10,"Dif",6,"Tours",3,"P",3,"O");
    printf("|_______|____________|____________|____________|____________|____________|________|_____|_____|\n");

    for(int i=0; i<NBRVOITURE; i++){
        dif = diffTempsFinal(i);

        if(copyCar[i].pit==1){
            pit="PIT";
        }else{
            pit=" ";
        }
        if(copyCar[i].out==1){
            out="OUT";
        }else{
            out=" ";
        }

        printf("| %*d | %*d | %*.3f | %*.3f | %*.3f | %*.3f | %*d | %*s | %*s |\n",
                   5,i+1,10,copyCar[i].pilote,10,(float)copyCar[i].secteur[0]/1000,10, (float)copyCar[i].secteur[1]/1000,10,(float)copyCar[i].secteur[2]/1000,10,dif/1000,6,copyCar[i].tour,3, pit,3, out);

    }

    printf("\n| Meilleur tour |   Voiture %d\t [%.3f]   |\n",copyCar[quiMeilleurLap()].pilote, (float)copyCar[quiMeilleurLap()].meilleurTour/1000);

    printf("\n| Best S1  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(1)].pilote, (float)copyCar[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
    printf("| Best S2  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(2)].pilote, (float)copyCar[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
    printf("| Best S3  \t|   Voiture %d\t [%.3f]   |\n", copyCar[quiMeilleurSecteur(3)].pilote, (float)copyCar[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    printf("\n");

}


void sauvegardeClassement(Voiture *vdata, int typeCourse,int NBRVOITCOURSE) {
    int file;
    for (int i=0; i<NBRVOITCOURSE;i++) {
        copyCar[i] = vdata[i];
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

        	qsort(copyCar, NBRVOITCOURSE, sizeof(Voiture), fctTriFinal);

        	for(int i =0 ; i<NBRVOITCOURSE; i++){
            	sprintf(buffer,"N°%d \t% d \n",i+1,copyCar[i].pilote);
            	write(file,buffer, strlen(buffer));
        	}

        	sprintf(buffer,"Meilleur tour \t Voiture %d \t [%.3f] \n",copyCar[quiMeilleurLap()].pilote, (float)copyCar[quiMeilleurLap()].meilleurTour/1000);
			write(file,buffer, strlen(buffer));
			sprintf(buffer,"Best S1 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(1)].pilote, (float)copyCar[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
			write(file,buffer, strlen(buffer));
			sprintf(buffer,"Best S2 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(2)].pilote, (float)copyCar[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
			write(file,buffer, strlen(buffer));
    		sprintf(buffer,"Best S3 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(3)].pilote, (float)copyCar[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    		write(file,buffer, strlen(buffer));
    		close(file);
			break;
	}

    char buff[2048];

    qsort(copyCar, NBRVOITCOURSE, sizeof(Voiture), fctTri);

    for(int i =0 ; i<NBRVOITCOURSE; i++){
        sprintf(buff,"%d\n",copyCar[i].pilote);
        write(file,buff, strlen(buff));
    }
	
	sprintf(buff,"Meilleur tour \t Voiture %d \t [%.3f] \n",copyCar[quiMeilleurLap()].pilote, (float)copyCar[quiMeilleurLap()].meilleurTour/1000);
	write(file,buff, strlen(buff));
	sprintf(buff,"Best S2 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(1)].pilote, (float)copyCar[quiMeilleurSecteur(1)].meilleurSecteur[0]/1000);
	write(file,buff, strlen(buff));
	sprintf(buff,"Best S2 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(2)].pilote, (float)copyCar[quiMeilleurSecteur(2)].meilleurSecteur[1]/1000);
	write(file,buff, strlen(buff));
    sprintf(buff,"Best S3 \t Voiture %d \t [%.3f] \n", copyCar[quiMeilleurSecteur(3)].pilote, (float)copyCar[quiMeilleurSecteur(3)].meilleurSecteur[2]/1000);
    write(file,buff, strlen(buff));
    close(file);
}




