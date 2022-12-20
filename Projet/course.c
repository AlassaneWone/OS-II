#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "voiture.h"
#include "const.h"
#include "random.h"

void course(Voiture *v,int numVoiture,int tempsCourse,sem_t *sem){

    srand(getpid());

    sem_wait(sem);
    v->pilote = numVoiture;
    v->etat = 0;
    v->tour = 0;
    v->tempTotal = 0;
    sem_post(sem);


    while (v->tempTotal < tempsCourse) {

        sem_wait(sem);
        if(crash() || v->out == 1){//Si crach
            if(v->out!=1 && v->tour > 2){
                switch (getRandomSecteur()) {
                    case 1 :
                        v->secteur[0] = 0;
                        v->secteur[1] = 0;
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                    case 2 :
                        v->secteur[0] = genererTemps(v->tour);
                        v->secteur[1] = 0;
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                    case 3 :
                        v->secteur[0] = genererTemps(v->tour);
                        v->secteur[1] = genererTemps(v->tour);
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                }

            }
        }

		else{
            v->secteur[0] = genererTemps(v->tour);
            v->secteur[1] = genererTemps(v->tour);

            //Si Arrèt au pit
			v->probaStandActuel = PROBASTAND;
            if(stand() == v->probaStandActuel){
                v->secteur[2] = genererTemps(v->tour)+randomStandTime();
                v->pit = 1;
                v->etat = 1;
            }
            else{
                v->pit = 0 ;
                v->etat = 0;
                v->secteur[2] = genererTemps(v->tour);
            }


            //Ajout du temps total
            v->tempTotal += v->secteur[0]+v->secteur[1]+v->secteur[2];

            //Ajout d'un tour
            v->tour += 1;

            //Récupération et Allocation des meilleurs scores
            if (v->secteur[0] < v->meilleurSecteur[0] || v->meilleurSecteur[0]==0) {
                v->meilleurSecteur[0] = v->secteur[0];
            }
            if (v->secteur[1] < v->meilleurSecteur[1] || v->meilleurSecteur[1]==0) {
                v->meilleurSecteur[1] = v->secteur[1];
            }
            if (v->secteur[2] < v->meilleurSecteur[2] || v->meilleurSecteur[2]==0) {
                v->meilleurSecteur[2] = v->secteur[2];
            }

            //Récupération du meilleur temps de tour
            if (v->secteur[0] + v->secteur[1] + v->secteur[2] < v->meilleurTour || v->meilleurTour == 0) {
                v->meilleurTour = v->secteur[0] + v->secteur[1] + v->secteur[2];
            }
        }
        sem_post(sem);

        sleep(1);
    }
}
void coursefinal(Voiture *v,int numVoiture,sem_t *sem) {

    srand(getpid());

    sem_wait(sem);
    v->pilote = numVoiture;
    v->etat = 0;
    v->tour = 0;
    v->tempTotal = 0;
    sem_post(sem);


    while (v->tour <= TOURDEFINAL - 1) {

        sem_wait(sem);

        if (crash() || v->out == 1) {//Si crach
            if (v->out != 1 && v->tour > 2) {
                switch(getRandomSecteur()) {
                    case 1 :
                        v->secteur[0] = 0;
                        v->secteur[1] = 0;
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                    case 2 :
                        v->secteur[0] = genererTemps(v->tour);
                        v->secteur[1] = 0;
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                    case 3 :
                        v->secteur[0] = genererTemps(v->tour);
                        v->secteur[1] = genererTemps(v->tour);
                        v->secteur[2] = 0;
                        v->out = 1;
                        v->etat = 2;
                        break;
                }

            }

        } else{
            v->secteur[0] = genererTemps(v->tour);
            v->secteur[1] = genererTemps(v->tour);

            //Si Arrèt au pit

			v->probaStandActuel = PROBASTAND;
            if(stand() == v->probaStandActuel){
                v->secteur[2] = genererTemps(v->tour)+randomStandTime();
                v->pit = 1;
                v->etat = 1;
            }
            else{
                v->pit = 0 ;
                v->etat = 0;
                v->secteur[2] = genererTemps(v->tour);
            }


            //Ajout du temps total
            v->tempTotal += v->secteur[0]+v->secteur[1]+v->secteur[2];

            //Ajout d'un tour
            v->tour += 1;

            //Récupération et Allocation des meilleurs scores
            if (v->secteur[0] < v->meilleurSecteur[0] || v->meilleurSecteur[0]==0) {
                v->meilleurSecteur[0] = v->secteur[0];
            }
            if (v->secteur[1] < v->meilleurSecteur[1] || v->meilleurSecteur[1]==0) {
                v->meilleurSecteur[1] = v->secteur[1];
            }
            if (v->secteur[2] < v->meilleurSecteur[2] || v->meilleurSecteur[2]==0) {
                v->meilleurSecteur[2] = v->secteur[2];
            }

            //Récupération du meilleur temps de tour
            if (v->secteur[0] + v->secteur[1] + v->secteur[2] < v->meilleurTour || v->meilleurTour == 0) {
                v->meilleurTour = v->secteur[0] + v->secteur[1] + v->secteur[2];
            }
        }

        sem_post(sem);

        sleep(1);

    }
}


