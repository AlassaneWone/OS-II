#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct voiture{
	float temps1;
	float temps2;
	float temps3;
	float tempsTot;
	int numVoiture;
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

float* Randoms(int lower, int upper,
                            int count)
{
    int i;
    int num;
    static float array[4];
    float somme =0; //Calcul le temps du tour complet
    
    for (i = 0; i < count; i++) {
        num = (rand() %
        (upper - lower + 1)) + lower;
        
        float temps = (num *0.001); //récupère le temps fait
    
    	array[i] = temps; //  met le temps dans un array
    	
    	somme += temps;
    }
    array[3] = somme;
    
    return array;
}


struct voiture voitureTour(struct voiture v)
{
	int k =0;
	
	float bestScore[4] = {50,50,50,500} ;
	int i=0;
	
	int lower = 25000, upper = 45000, count = 3; // temps min/max possible et nombre d'itérations ( 3 séquences sur 1 tour)
	
	for ( k = 0; k < 10; k++ ) {
	
      float *score = Randoms(lower, upper, count);
      int j=0;
      
      for(j = 0; j<4; j++){

      
      	if(score[j] < bestScore[j]){
      		bestScore[j] = score[j];
      	}
      	
      	//printf( " tour %i  sécteur 	%i  numéro atm: %.3fs \n", k+1,j+1, score[j]);
      }
      
      //printf("Bestscore atm : %.3f \n", bestScore[3]);
    }
	
	for ( i = 0; i < 3; i++ ) {
      //printf( "Meilleur temps sécteur numéro %i : %.3fs \n",i+1, bestScore[i]);
    }
    
    //printf("temps total tour : %.3fs \n", bestScore[3]);
		
	v.temps1 = bestScore[0];
	v.temps2 = bestScore[1];
	v.temps3 = bestScore[2];
	v.tempsTot = bestScore[3];
	return v;
}

void meilleurTemps(struct voiture voitures[10]){
	int i;
	struct meilleurTemps meilleur;
	meilleur.temps1 = 50;
	meilleur.temps2 = 50;
	meilleur.temps3 = 50;
	meilleur.tempsTot = 500;
	
	for(i=0; i <10; i++){
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
	
	printf("Les meilleurs temps sont S1: %.3lf par voiture n%i, S2: %.3lf par voiture n%i, S3: %.3lf par voiture n%i, Tot: %.3lf par voiture n%i \n", meilleur.temps1, meilleur.voitS1, 		 meilleur.temps2, meilleur.voitS2, meilleur.temps3, meilleur.voitS3, meilleur.tempsTot, meilleur.voitTot);
	
}

int main()
{
	struct voiture voitures[10];
	struct voiture v;
	int l;
	
	for(l= 0; l<10; l++){
		srand(time(0)+l);
		voitures[l] = voitureTour(v);
		voitures[l].numVoiture = l;
			printf("Vérification des valeurs dans main de voiture %i: %.3lf %.3lf %.3lf %.3lf \n", l, voitures[l].temps1, voitures[l].temps2, voitures[l].temps3, voitures[l].tempsTot);
	}
	meilleurTemps(voitures);
	return 0;
}

