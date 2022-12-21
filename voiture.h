
#ifndef PROJET_VOITURE
#define PROJET_VOITURE

// Définifs une nouvelle strucutre, voiture
typedef struct voiture{
    int etat;             	
    //status de la voiture : 0 = enCourse, 1 = stand , 2 =crash
    int pilote;                
    //Numero de pilote
    float secteur[3];         
    //List temps des Secteurs
    float meilleurSecteur[3];     
    //Meilleur temps pour chaque secteur (n° = index+1)
    float meilleurTour;            
    //meilleur tour
    int tempTotal;          
    //Temps de course
    int tour;               
    //Numéro du tour actuel
    int pit;              
    //1 = au pit
    int out;                
    //1 = crach
}Voiture;

#endif
