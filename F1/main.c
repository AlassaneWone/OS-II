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
#include "config.h"
#include "course.h"
#include "voiture.h"
#include "result.h"

int TEMPSCOURSE = 5400000;            //Temps de la course selectionnée
int pilotes[20];   //Numéros de voitures



int main(){
    Voiture *voit ;

    // mémoire partagée
    int shmid = shmget(IPC_PRIVATE, sizeof(Voiture) * NBRVOITURE, 0666 | IPC_CREAT);
    //attache le segment de mémoire partagée à car
    voit = shmat(shmid, 0, 0);

    //création et initiation du sémaphore
    int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
    sem_t *semaphore = shmat(shmid_sem, 0, 0);
    sem_init(semaphore, 1, 1);


    int i = 0;
    pid_t pid;
    numVoiture(voit);


    //Processus Fils
    for (i; i < NBRVOITURE; i++) {
    	pid = fork();
        if (pid == 0) break;
    }
    if (pid == -1) {
    	perror("Erreur fork ");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
    	course(&voit[i], pilotes[i], TEMPSCOURSE, semaphore);

        exit(0);
	}

    //Proccessus Père
    else {
    	bool is_course_finie = false;
            int nmbr_voiture_finie = 0;
            while (!is_course_finie) {
                    afficheResult(voit, semaphore);
                    for (int j = 0; j < NBRVOITURE; j++) {
                    	// compte le nombre de voiture qui ont finit les essais
                        if (voit[j].tempTotal > TEMPSCOURSE|| voit[j].etat == 2) {
                            nmbr_voiture_finie++;
                        }
                        if (nmbr_voiture_finie == NBRVOITURE) {
                            is_course_finie = true;
                        }
                    }
                    sleep(DELAY);
            }
            shmdt(voit); 
            shmctl(shmid, IPC_RMID, NULL); 

            sem_destroy(semaphore);
            exit(0);
        }
}




void numVoiture(Voiture *vdata){


    int pilote[NBRVOITURE] = {44,77,11,33,3,4,5,18,14,31,16,55,10,22,7,99,9,47,6,63};  //Numéros de voitures


    for(int j=0;j<NBRVOITURE;j++){
    	pilotes[j] = pilote[j];
    }

}
