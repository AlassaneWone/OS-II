#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct voiture{
	float temps1;
	float temps2;
	float temps3;
	float tempsTot;
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



int main()
{
	struct voiture voitures[10];
	struct voiture v;
	int l;
	
	for(l= 0; l<10; l++){
		srand(time(0)+l);
		voitures[l] = voitureTour(v);
			printf("Vérification des valeurs dans main de voiture %i: %.3lf %.3lf %.3lf %.3lf \n", l, voitures[l].temps1, voitures[l].temps2, voitures[l].temps3, voitures[l].tempsTot);
	}

	return 0;
}

