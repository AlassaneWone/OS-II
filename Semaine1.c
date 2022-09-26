#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


float* printRandoms(int lower, int upper,
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
    	//printf("%.3f ", array[i]); // print à 3décimal les temps
    	somme += temps;
    }
    array[3] = somme;
    
    return array;
}


int main()
{
	int k =0;
	
	float bestScore[4] = {50,50,50,500} ;
	int i=0;
	
	int lower = 25000, upper = 45000, count = 3; // temps min/max possible et nombre d'itérations
	
	
	
	for ( k = 0; k < 10; k++ ) {
	
	  
      float *score = printRandoms(lower, upper, count);
      int j=0;
      
      for(j = 0; j<4; j++){
      srand(time(0)+k+j);
      	if(score[j] < bestScore[j]){
      		bestScore[j] = score[j];
      	}
      	printf( " tour %i  sécteur 	%i  numéro atm: %.3fs \n", k+1,j+1, score[j]);
      }
      printf("Bestscore atm : %.3f \n", bestScore[3]);
      
    }
	
	
	for ( i = 0; i < 3; i++ ) {
      printf( "Meilleur temps sécteur numéro %i : %.3fs \n",i+1, bestScore[i]);
      
    }
    printf("temps total tour : %.3fs \n", bestScore[3]);
		
	return 0;

}

