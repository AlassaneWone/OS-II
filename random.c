#include <stdlib.h>
#include "const.h"

int getRandomSecteur(){
    return rand() % (3 + 1 - 1) + 1;
    //Permet d'avoir un secteur random pour le crash
}


int lower = 25000, upper = 45000;
float tempsRandom(){
	// Random entre 25000 et 45000
    return rand() % (upper + 1 - lower) + lower;

}
int randomPitTime(){
	//Random entre 20000 et 30000
    return rand() % (MAXPIT + 1 - MINPIT) + MINPIT;
}

int crash(){
	// Return 1 si Crash, sinon 0
    if(rand()% PROBACRASH == 1)
		return 1;

    return 0;
}
int tourRandom(){
	//Récupère un tour random 
	 return rand() % (10 + 1 - 1) + 1;
}

int stand(){
	// Return 1 si Pit, sinon 0
    if(rand()% PROBAPIT == 1)
		return 1;

    return 0;
}


