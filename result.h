

#ifndef PROJETOS_RESULT_H
#define PROJETOS_RESULT_H

void afficheResult(Voiture *v,sem_t *semaphore);
void afficheResultFinal(Voiture *v,sem_t *semaphore);
int tri(const void *a, const void *b);
int triFinal(const void *a, const void *b);
int getBestSecteur(int secteur);
int getBestLap();
int getDiff(int i);
void saveToFile(Voiture *v,char *argv[],int NBRVOItURE);

#endif //PROJETOS_RESULT_H
