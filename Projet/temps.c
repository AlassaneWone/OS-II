#include <stdlib.h>
#include "const.h"

int lower = 25000, upper = 45000, count = 3;

int genererTemps(int nbrTour){

    return rand() % (upper + 1 - lower) + lower;

}

int crash(){
    if(rand()% PROBACRACH == 1)
		return 1;

    return 0;
}

int getRandomSecteur(){
    return rand() % (3 + 1 - 1) + 1;
    //Permet d'avoir un secteur random pour le crash
}

int stand(){
    return rand() % PROBASTAND+1;
}

int randomStandTime(){
    return rand() % (MAXSTANDTIME + 1 - MINSTANDTIME) + MINSTANDTIME;
}
