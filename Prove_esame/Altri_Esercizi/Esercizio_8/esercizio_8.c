/**
 * ESERCIZIO: 8
 * 
 * Si progetti un programma in ambiente Linux che, impiegando  la  libreria  pthread,
 * crei  un  thread  che  estrae N volte ( con N passato da riga di comando )
 * un numero casuale compreso tra 0 e 10 e ne calcoli il fattoriale  ponendolo 
 * in  una  variabile  condivisa e  un  thread  che  stampi  a  video  il  fattoriale 
 * calcolato  N  volte prelevandolo dalla variabile condivisa.
 *
 * Per la sincronizzazione utilizzare i semafori con nome
*/

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int N;
int fattoriale(int);
int num;
int fatt;

//dichiaro il nome del semaforo
const char* semaphore_writ = "/writ";
const char* semaphore_prin = "/prin";
sem_t *writ;
sem_t *prin;


void *extract(void *data)
{
    for(int i = 0; i<N; i++)
    {
        sem_wait(writ);
        
            num = rand()%11;
            printf("Ho estratto il %d-esimo numero %d\n", i, num);
            fatt = fattoriale(num);

        sem_post(prin);
    }

    pthread_exit(NULL);

}


void *print(void *data)
{
    for(int i = 0; i<N; i++)
    {
        sem_wait(prin);
        
            printf("Il fattoriale di %d è %d\n", num, fatt);
        
        sem_post(writ);
    }
}


int main(int argc, char *argv[])
{   

    N = atoi(argv[1]);

    sem_unlink(semaphore_writ);
    sem_unlink(semaphore_prin);

    writ = sem_open(semaphore_writ, O_CREAT|O_EXCL,0644, 1);
    prin = sem_open(semaphore_prin, O_CREAT|O_EXCL,0644, 0);

    pthread_t extractor, printer;

    pthread_create(&extractor, NULL, extract, NULL);
    pthread_create(&printer, NULL, print, NULL);

    pthread_join(extractor, NULL);
    pthread_join(printer, NULL);

    sem_close(writ);
    sem_close(prin);

    sem_unlink(semaphore_writ);
    sem_unlink(semaphore_prin);

    exit(1);
}







int fattoriale(int number)
{
    if (number == 0 || number == 1) {
        return 1;
    } else {
        return number * fattoriale(number - 1);
    }
}