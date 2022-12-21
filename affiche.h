

#ifndef PROJET_AFFICHE
#define PROJET_AFFICHE

void affiche(Voiture *voitPartage,sem_t *semaphore, int NBRVOITCOURSE);
int fctTri(const void *a, const void *b);
int quiMeilleurSecteur(int secteur);
int quiMeilleurTour();
int diffTemps(int i);
void sauvegardeClassement(Voiture *voitPartage, int typeCourse,int NBRVOITCOURSE);
void afficheFinal(Voiture *voitPartage,sem_t *semaphore);

#endif 
