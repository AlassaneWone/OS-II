#include <stdlib.h>
#include "config.h"

int lower = 25000, upper = 45000, count = 3;

int genererTemps(int nbrTour){

    return rand() % (upper + 1 - lower) + lower;

}

int crachTest(){
    if(rand()% PROBACRACH == 1)
		return 1;

    return 0;
}

int getRandomSecteur(){
    return rand() % (3 + 1 - 1) + 1;
}

int stand(){
    return rand() % PROBASTAND+1;
}

int randomStandTime(){
    return rand() % (MAXSTANDTIME + 1 - MINSTANDTIME) + MINSTANDTIME;
}
