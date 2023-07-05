/**
 * ESERCIZIO: 1
 * 
 * Facendo uso della libreria Pthread( fatto con i semafori per esercizio ),
 * 
 * realizzare un programma in cui un thread scrittore,
 * dato un intero N da riga di comando ( dove 10 < N <= 15 ),
 * scrive in un file nella prima posizione,
 * 
 * uno alla volta ed ogni ½ secondo, la sequenza di Fibonacci di ordine N, 
 * alternandosi con un thread lettore che legge, uno alla volta dalla
 * prima posizione del file, i numeri scritti dal thread scrittore.
 * 
 * Un terzo thread attende la lettura dell’ N-esimo intero, quindi
 * stampa a video il messaggio 
 * “Operazioni concluse, arrivederci dal thread: tid”,
 * attende 5 secondi e termina.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define SEM = "/sem"


typedef struct{
    int number;
    pthread_mutex_t mutex;
    sem_t *sem;
}data;

data *ptr;

void alloc_struct();



void alloc_struct()
{
    ptr = malloc(sizeof(data));

    ptr->number = 0;
    pthread_mutex_init(&ptr->mutex, NULL);
    ptr->sem = sem_open(SEM, O_CREAT|O_EXCL, S_IWUSR | S_IRUSR)

}