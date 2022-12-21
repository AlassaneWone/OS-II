#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include "voiture.h"
#include "const.h"
#include "random.h"


void course(Voiture *voiture,int numVoiture,int tempsCourse, sem_t *sem){

    srand(getpid());

    sem_wait(sem);
    // Au début de la course, on met les états initiaux
    voiture->pilote = numVoiture;
    voiture->etat = 0;
    voiture->tour = 0;
    voiture->tempTotal = 0;
    sem_post(sem);


    while (voiture->tempTotal < tempsCourse) {

        sem_wait(sem);
        if(crash() || voiture->out == 1){//Si crash
            if(voiture->out!=1 && voiture->tour > 2){
            	int randomSecteur = getRandomSecteur();
				if (randomSecteur == 1) {
					// Si on se crash au S1, pas de temps ailleurs
  					voiture->secteur[0] = 0;
  					voiture->secteur[1] = 0;
  					voiture->secteur[2] = 0;
				}else if (randomSecteur == 2) {
					// Si on se crash au S2, temps uniquement au S1
					voiture->secteur[0] = tempsRandom();
					voiture->secteur[1] = 0;
					voiture->secteur[2] = 0;

				}else if (randomSecteur == 3) {
					// Si on se crash au S3, temps uniquement au S1 et S2
					voiture->secteur[0] = tempsRandom();
					voiture->secteur[1] = tempsRandom();
					voiture->secteur[2] = 0;

				}
				voiture->out = 1;
				voiture->etat = 2;
				 
            }
        }

		else{
            voiture->secteur[0] = tempsRandom();
            voiture->secteur[1] = tempsRandom();

            //Si Arrèt au pit
            if(stand()){
            	// Le pit est uniquement au S3
                voiture->secteur[2] = tempsRandom()+randomPitTime();
                voiture->pit = 1;
                voiture->etat = 1;
            }
            else{
                voiture->pit = 0 ;
                voiture->etat = 0;
                voiture->secteur[2] = tempsRandom();
            }


            //Ajout du temps total
            voiture->tempTotal += voiture->secteur[0]+voiture->secteur[1]+voiture->secteur[2];

            //Ajout d'un tour
            voiture->tour += 1;

            //Récupération et Allocation des meilleurs scores de la voitures
            if (voiture->secteur[0] < voiture->meilleurSecteur[0] || voiture->meilleurSecteur[0]==0) {
                voiture->meilleurSecteur[0] = voiture->secteur[0];
            }
            if (voiture->secteur[1] < voiture->meilleurSecteur[1] || voiture->meilleurSecteur[1]==0) {
                voiture->meilleurSecteur[1] = voiture->secteur[1];
            }
            if (voiture->secteur[2] < voiture->meilleurSecteur[2] || voiture->meilleurSecteur[2]==0) {
                voiture->meilleurSecteur[2] = voiture->secteur[2];
            }

            //Récupération du meilleur temps de tour
            int tempsTourActuel = voiture->secteur[0] + voiture->secteur[1] + voiture->secteur[2];
            if (tempsTourActuel < voiture->meilleurTour || voiture->meilleurTour == 0) {
                voiture->meilleurTour = tempsTourActuel;
            }
        }
        sem_post(sem);

        sleep(1); //Delai entre chaque tour sinon on ne peut pas voir l'évolution de la course !
    }
}
void coursefinal(Voiture *voiture,int numVoiture,sem_t *sem) {

    srand(getpid());
	// Au début de la course, on met les états initiaux
    sem_wait(sem);
    voiture->pilote = numVoiture;
    voiture->etat = 0;
    voiture->tour = 0;
    voiture->tempTotal = 0;
    sem_post(sem);


    while (voiture->tour <= TOURDEFINAL - 1) {

        sem_wait(sem);

        if (crash() || voiture->out == 1) {//Si crach
            if (voiture->out != 1 && voiture->tour > 2) {
                int randomSecteur = getRandomSecteur();
				if (randomSecteur == 1) {
  					voiture->secteur[0] = 0;
  					voiture->secteur[1] = 0;
  					voiture->secteur[2] = 0;
				} else if (randomSecteur == 2) {
					voiture->secteur[0] = tempsRandom();
					voiture->secteur[1] = 0;
					voiture->secteur[2] = 0;
				} else if (randomSecteur == 3) {
					voiture->secteur[0] = tempsRandom();
					voiture->secteur[1] = tempsRandom();
					voiture->secteur[2] = 0;
				}
				voiture->out = 1;
				voiture->etat = 2;
            }

        } else{
            voiture->secteur[0] = tempsRandom();
            voiture->secteur[1] = tempsRandom();

            //Si Arrèt au pit
            if(stand()){
            	//Pit uniquement en s3
                voiture->secteur[2] = tempsRandom()+randomPitTime();
                voiture->pit = 1;
                voiture->etat = 1;
            }
            else{
                voiture->pit = 0 ;
                voiture->etat = 0;
                voiture->secteur[2] = tempsRandom();
            }


            //Ajout du temps total
            voiture->tempTotal += voiture->secteur[0]+voiture->secteur[1]+voiture->secteur[2];

            //Ajout d'un tour
            voiture->tour += 1;

            //Récupération et Allocation des meilleurs scores
            if (voiture->secteur[0] < voiture->meilleurSecteur[0] || voiture->meilleurSecteur[0]==0) {
                voiture->meilleurSecteur[0] = voiture->secteur[0];
            }
            if (voiture->secteur[1] < voiture->meilleurSecteur[1] || voiture->meilleurSecteur[1]==0) {
                voiture->meilleurSecteur[1] = voiture->secteur[1];
            }
            if (voiture->secteur[2] < voiture->meilleurSecteur[2] || voiture->meilleurSecteur[2]==0) {
                voiture->meilleurSecteur[2] = voiture->secteur[2];
            }

            //Récupération du meilleur temps de tour
            int tempsTourActuel = voiture->secteur[0] + voiture->secteur[1] + voiture->secteur[2];
            if ( tempsTourActuel < voiture->meilleurTour || voiture->meilleurTour == 0) {
                voiture->meilleurTour = tempsTourActuel;
            }
        }

        sem_post(sem);

        sleep(1); //Delai entre chaque tour sinon on ne peut pas voir l'évolution de la course !

    }
}


