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
    	//printf("%.3f ", array[i]); // print à 3déciaml les temps
    	somme += temps;
    }
    array[3] = somme;
    
    return array;
}

int main()
{
	int i=0;
	float *score;
	float lower = 25000, upper = 45000, count = 3; // temps min/max possible et nombre d'itérations
	srand(time(0));
	score = printRandoms(lower, upper, count);
	for ( i = 0; i < 3; i++ ) {
      printf( "Temps secteur numéro %i : %.3fs  \n",i+1, score[i]);
    }
    printf("Temps total tour : %.3fs  \n", score[3]);
	
	
	
	return 0;
	

}