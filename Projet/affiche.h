

#ifndef PROJET_AFFICHE
#define PROJET_AFFICHE

void affiche(Voiture *v,sem_t *semaphore, int NBRVOITCOURSE);
int fctTri(const void *a, const void *b);
int quiMeilleurSecteur(int secteur);
int quiMeilleurLap();
int diffTemps(int i);
void sauvegardeClassement(Voiture *vdata, int typeCourse,int NBRVOITCOURSE);
void afficheFinal(Voiture *vdata,sem_t *semaphore);

#endif 
