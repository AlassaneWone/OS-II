
#ifndef PROJETOS_VOITURE_H
#define PROJETOS_VOITURE_H

// VOITURE struct
typedef struct voiture{
    int etat;             //status de la voiture : 0 = enCourse, 1 = stand , 2 =crach
    int num;                //Numero de voiture
    int secteur[3];         //List temps des Secteurs
    int bestSecteur[3];     //Best temps pour chaque secteur (n° = index+1)
    int bestLap;            //Best Lap
    int tempTotal;          //Temps de course
    int tour;               //Numéro du tour actuel
    int stand;              //1 = au stand
    int out;                //1 = crach
    int probaStand;         //Probabilité d'un arrèt au stand
}Voiture;

#endif //PROJETOS_VOITURE_H
