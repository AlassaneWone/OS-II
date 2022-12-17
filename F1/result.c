
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "config.h"
#include "voiture.h"

Voiture copyCar[NBRVOITURE];


int whoBestSecteur(int secteur){
    int indexOfBestCar = 0;
    int bestTime = 0;

    for(int i=0;i<NBRVOITURE;i++){
        if(secteur==1){
            if( copyCar[i].bestSecteur[0] != 0 &&(copyCar[i].bestSecteur[0] < bestTime || bestTime == 0 )){
                bestTime = copyCar[i].bestSecteur[0];
                indexOfBestCar = i;
            }
        }
        else if(secteur==2){
            if( copyCar[i].bestSecteur[1] != 0 &&(copyCar[i].bestSecteur[1] < bestTime || bestTime == 0)){
                bestTime = copyCar[i].bestSecteur[1];
                indexOfBestCar = i;
            }
        }
        else if(secteur == 3){
            if(copyCar[i].bestSecteur[2] != 0 &&(copyCar[i].bestSecteur[2] < bestTime || bestTime == 0  )){
                bestTime = copyCar[i].bestSecteur[2];
                indexOfBestCar = i;
            }
        }

    }
    return indexOfBestCar;
}

int whoBestLap(){
    int index = 0;
    int best = 0;

    for(int i=0; i < NBRVOITURE; i++){
        if( copyCar[i].bestLap != 0 &&(copyCar[i].bestLap < best || best == 0)){
            best = copyCar[i].bestLap;
            index = i;
        }
    }

    return index;
}

///////////////////////////////////////////////////////////////////////////////////////////Essais et Qualifs
int tri(const void *a, const void *b){
    const Voiture *p1 = (Voiture *) a;
    const Voiture *p2 = (Voiture *) b;

    if(p1->bestLap==0){
        return 1;
    }
    else if(p2->bestLap==0){
        return -1;
    }
    else if(p1->bestLap > p2->bestLap){
        return 1;
    } else if(p1->bestLap < p2->bestLap){
        return -1;
    } else {
        return 0;
    }
}

int getDiff(int i){
    if (i != 0 && copyCar[i].bestLap != 0){
        return -(copyCar[i-1].bestLap - copyCar[i].bestLap);
    }else
    {
        return 0;
    }
}


void afficheResult(Voiture *vdata,sem_t *semaphore){
    float dif;
    system("clear");
    char *stand;
    char *out;

    sem_wait(semaphore);

    for (int i=0; i<NBRVOITURE;i++) {
        copyCar[i] = vdata[i];
    }
    sem_post(semaphore);

    qsort(copyCar, NBRVOITURE, sizeof(Voiture), tri);

    ///CREATION DU TABLEAU
    printf("\n");
    printf("| %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s | %*s |\n",5,"Pos.",10,"Numero",10,"Secteur 1",10,"Secteur 2",10,"Secteur 3",10,"BestLap",10,"Dif",6,"Tours",3,"P",3,"O");
    printf("\n");


    for(int i=0; i<NBRVOITURE; i++){
        dif = getDiff(i);
        if(copyCar[i].stand==1){
            stand="P";
        }else{
            stand=" ";
        }
        if(copyCar[i].out==1){
            out="O";
        }else{
            out=" ";
        }
        if(copyCar[i].num > 0 && copyCar[i].num<100){
            printf("| %*d | %*d | %*.3f | %*.3f | %*.3f | %*.3f | %*.3f | %*d | %*s | %*s |\n",
                   5,i+1,10,copyCar[i].num,10,(float)copyCar[i].secteur[0]/1000,10, (float)copyCar[i].secteur[1]/1000,10,(float)copyCar[i].secteur[2]/1000,10,(float)copyCar[i].bestLap/1000,10,dif/1000,6,copyCar[i].tour,3, stand,3, out);
        }

    }


    printf("\n| Meilleur tour |   Voiture %d\t [%.3f]   |\n",copyCar[whoBestLap()].num, (float)copyCar[whoBestLap()].bestLap/1000);

    printf("\n| Best S1  \t|   Voiture %d\t [%.3f]   |\n", copyCar[whoBestSecteur(1)].num, (float)copyCar[whoBestSecteur(1)].bestSecteur[0]/1000);
    printf("| Best S2  \t|   Voiture %d\t [%.3f]   |\n", copyCar[whoBestSecteur(2)].num, (float)copyCar[whoBestSecteur(2)].bestSecteur[1]/1000);
    printf("| Best S3  \t|   Voiture %d\t [%.3f]   |\n", copyCar[whoBestSecteur(3)].num, (float)copyCar[whoBestSecteur(3)].bestSecteur[2]/1000);
    printf("\n");

}


int getDiffFinal(int i){
    if(copyCar[i].out==1){
        return 0;
    }
    else if (i != 0 ){
        return -(copyCar[i-1].tempTotal - copyCar[i].tempTotal);
    }else
    {
        return 0;
    }
}



