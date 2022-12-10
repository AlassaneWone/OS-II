#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>

struct voiture{
	float temps1;
	float temps2;
	float temps3;
	float tempsTot;
	int numVoiture;
	float bestLap;
};

struct meilleurTemps{
	float temps1;
	float temps2;
	float temps3;
	float tempsTot;
	int voitS1;
	int voitS2;
	int voitS3;
	int voitTot;
};

int shmid;


float* Randoms(int lower, int upper,int count){
    int i;
    int num;
    static float array[4];
    float somme =0; //Calcul le temps du tour complet
    
    for (i = 0; i < count; i++) {
        num = (rand() %
        (upper - lower + 1)) + lower;
        
        float temps = (num); //récupère le temps fait
    
    	array[i] = temps; //  met le temps dans un array
    	
    	somme += temps;
    }
    array[3] = somme;
    
    return array;
}

const char* Timer(float millis) {
    //int millis=789562;
    float secondes=0;
    float minutes=0;
    float heures=0;
    char rep;
    while(millis >= 1000){
        millis-=1000;
        secondes++;
        if(secondes==60){
            secondes-=60;
            minutes++;
        }
        if(minutes==60){
            minutes-=60;
            heures++;
        }
    }
    //char heure[100];
  	//char minute[100];
  	//char seconde[100];
  	//char mili[100];
  	
    //gcvt(heures, 2, heure);
    //gcvt(minutes, 2, minute);
    //gcvt(secondes, 2, seconde);
  	//gcvt(millis, 2, milli);
  	
  	//char totale = heure + minute + seconde + milli;
  	//sprintf(str, "%c %c %c %c", heure, minute, seconde, milli );
  	
	printf("\n %.0f:%.0f:%.0f:%.0f \n", heures ,minutes, secondes ,millis);
	return 0;
	
}


void voitureTour(struct voiture *v, sem_t *sem)
{
	//prend une voiture en paramètre, lui met 4 temps randoms dans temps1/2/3/Tot
	sem_wait(sem);
	int k =0;
	srand(getpid()+time(NULL));
	float impScore[4] = {500000,500000,500000,5000000} ;
	int i=0;
	
	int lower = 25000, upper = 45000, count = 3; // temps min/max possible et nombre d'itérations ( 3 séquences sur 1 tour)
	
    float *score = Randoms(lower, upper, count); // Aeppelle Randoms qui return array[4]
    int j=0;
    
	v->temps1 = score[0];
	v->temps2 = score[1];
	v->temps3 = score[2];
	v->tempsTot = score[3];
	
	if (v->tempsTot < v->bestLap || v->bestLap == 0) {
    	v->bestLap = v->tempsTot;
    }
	sem_post(sem);
}

struct meilleurTemps trouveMeilleurTemps(struct voiture voitures[20]){
	int i;
	struct meilleurTemps meilleur;
	meilleur.temps1 = 500000;
	meilleur.temps2 = 50000;
	meilleur.temps3 = 50000;
	meilleur.tempsTot = 500000;
	
	for(i=0; i <20; i++){
		if(voitures[i].temps1 < meilleur.temps1){
			meilleur.temps1 = voitures[i].temps1;
			meilleur.voitS1 = voitures[i].numVoiture;
		}
		if(voitures[i].temps2 < meilleur.temps2){
			meilleur.temps2 = voitures[i].temps2;
			meilleur.voitS2 = voitures[i].numVoiture;
		}
		if(voitures[i].temps3 < meilleur.temps3){
			meilleur.temps3 = voitures[i].temps3;
			meilleur.voitS3 = voitures[i].numVoiture;
		}
		if(voitures[i].tempsTot < meilleur.tempsTot){
			meilleur.tempsTot = voitures[i].tempsTot;
			meilleur.voitTot = voitures[i].numVoiture;
		}
	}
	
	printf("Les meilleurs temps sont S1: %.3lf par voiture n%i, S2: %.3lf par voiture n%i, S3: %.3lf Par voiture n%i, Tot: %.3lf par voiture n%i \n ", meilleur.temps1/1000, meilleur.voitS1,meilleur.temps2/1000, meilleur.voitS2, meilleur.temps3/1000, meilleur.voitS3, meilleur.tempsTot/1000, meilleur.voitTot);
	
	return meilleur;
}

void meilleurDesMeilleurs(struct meilleurTemps meilleursTemps[5]){
	int i;
	struct meilleurTemps meilleur;
	meilleur.temps1 = 500000;
	meilleur.temps2 = 500000;
	meilleur.temps3 = 500000;
	meilleur.tempsTot = 5000000;
	
	for(i=0; i <5; i++){
		if(meilleursTemps[i].temps1 < meilleur.temps1){
			meilleur.temps1 = meilleursTemps[i].temps1;
			meilleur.voitS1 = meilleursTemps[i].voitS1;
		}
		if(meilleursTemps[i].temps2 < meilleur.temps2){
			meilleur.temps2 = meilleursTemps[i].temps2;
			meilleur.voitS2 = meilleursTemps[i].voitS2;
		}
		if(meilleursTemps[i].temps3 < meilleur.temps3){
			meilleur.temps3 = meilleursTemps[i].temps3;
			meilleur.voitS3 = meilleursTemps[i].voitS3;
		}
		if(meilleursTemps[i].tempsTot < meilleur.tempsTot){
			meilleur.tempsTot = meilleursTemps[i].tempsTot;
			meilleur.voitTot = meilleursTemps[i].voitTot;
		}
	}
	
	//printf("S1 par voiture n%i: \t",meilleur.voitS1);
	Timer(meilleur.temps1);
	
	//printf("S2 par voiture n%i: \t",meilleur.voitS2);
	Timer(meilleur.temps2);
	
	//printf("S3 par voiture n%i: \t",meilleur.voitS3);
	Timer(meilleur.temps3);
	
	//printf("S4 par voiture n%i: \t",meilleur.voitTot);
	Timer(meilleur.tempsTot);

}

void afficheResult(struct voiture *vdata,sem_t *semaphore){
	struct voiture copy_voit[20];
	struct meilleurTemps meilleurTemps;
	sem_wait(semaphore);
    for (int i=0; i<20;i++) {
        copy_voit[i] = vdata[i];
    }
    sem_post(semaphore);
    
    int j = 0;
	system("clear");  
	printf("Numéro de voiture\t tempsS1\t temps S2\t temps S3\t temps STOT");
	for (j = 0; j<20; j++){
    	printf("\n \t %i \t   \t%.3lf     \t%.3lf     \t%.3lf    \t %.3lf  ", copy_voit[j].numVoiture, copy_voit[j].temps1/1000, copy_voit[j].temps2/1000, copy_voit[j].temps3/1000, 				copy_voit[j].tempsTot/1000);
    	printf(" meilleur temps de la voiture %i : %.3lf ", copy_voit[j].numVoiture,copy_voit[j].bestLap/1000);
	}
	
	printf("\n");
	meilleurTemps = trouveMeilleurTemps(copy_voit);
}

int main()
{
	//mémmoire partagée
    shmid= shmget(60, 20*sizeof(struct voiture), IPC_CREAT|0666);
	struct voiture *circuit = shmat(shmid, 0, 0);
	//sémaphore
	int shmid_sem = shmget(IPC_PRIVATE, sizeof(sem_t), 0666 | IPC_CREAT);
    sem_t *semaphore = shmat(shmid_sem, NULL, 0);
    sem_init(semaphore, 1, 1);
        
	int m;
	for (m =0; m<20;m++){
		circuit[m].temps1 =0;
		circuit[m].temps2 =0;
		circuit[m].temps3 =0;
		circuit[m].tempsTot =0;
	}
	circuit[0].numVoiture = 59;
    circuit[1].numVoiture = 44;
    circuit[2].numVoiture = 69;
    circuit[3].numVoiture = 101;
    circuit[4].numVoiture = 420;
    circuit[5].numVoiture = 313;
    circuit[6].numVoiture = 19;
    circuit[7].numVoiture = 5;
    circuit[8].numVoiture = 4;
	circuit[9].numVoiture = 99;
	circuit[10].numVoiture = 10;
    circuit[11].numVoiture = 11;
    circuit[12].numVoiture = 12;
    circuit[13].numVoiture = 71;
    circuit[14].numVoiture = 448;
    circuit[15].numVoiture = 21;
    circuit[16].numVoiture = 68;
    circuit[17].numVoiture = 578;
    circuit[18].numVoiture = 44;
	circuit[19].numVoiture = 913;
	
	int i;
	pid_t pid ;

	//pids[0] = getpid();
	struct voiture v;  
	struct meilleurTemps meilleurTempsparTour[10];
	struct meilleurTemps test;
	
	for (int j = 0; j <10 ; j++){
	
		for (i = 0; i < 20; ++i) {
			pid = fork();
    		if (pid  < 0) {
   				perror("fork");
   				abort();	
    		} else if (pid == 0) {		  	
				//Fils
            	voitureTour(&circuit[i], semaphore);

        		exit(0);
    		}
		}
		afficheResult(circuit, semaphore);
	}
	sleep(1);
	
	

	shmdt(circuit);
	sem_destroy(semaphore);
	shmctl(shmid, IPC_RMID, NULL);
	
	
}
