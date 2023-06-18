/**
 * ESAME: Prova Scritta 20/06/2022
*/

/**
 * 9 punti ( obbligatorio )
 * 
 * Scrivere un programma C e Posix che prende da riga di comando N interi
 * compresi tra 5 e 20. Il valore di N è constante ed indicato nel sorgente.
 * 
 * Il programma avvia N thread che hanno il seguente comportamento:
 * 
 *  a) Ogni thread va in sospensione ( invocando la funzione sleep() ) per un numero
 *     di secondi pari al valore del corrispondente parametro e poi stampa il suo indice
 *  
 *  b) Alla fine del periodo di attesa ( sleep ), ogni thread stampa un messaggio: "Thread #: svegliato!".
 *  
 *  c) Tutti i thread si sincronizzano tra di loro ( utilizzando i semafori basati su memoria ).
 * 
 *  d) dopo aver atteso il termine di tutti gli altri thread, ciascun thread scrive sullo schermo 
 *     che ha terminato ( "Thread #: terminato..." ).
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define SIZE 10

int array[SIZE];
int count;

sem_t aspetta;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void *routine(void *data)
{
    int *tid = (int *) data;
    int val;

    sleep(*tid);


    pthread_mutex_lock(&mutex);
    printf("Thread #%d svegliato!\n", *tid);
    count++;        
    pthread_mutex_unlock(&mutex);

    //sem_wait(&aspetta);

    //sem_getvalue(&aspetta, &val);

    printf("val %d\n", val);
    
    pthread_mutex_lock(&mutex);
    if(count == SIZE)
    {
        pthread_mutex_unlock(&mutex);

        printf("Thread #%d terminato\n", *tid);

        pthread_cond_broadcast(&cond);

    } else
        {
            while(count < SIZE)
            {
                pthread_cond_wait(&cond, &mutex);
            }

            printf("Thread #%d terminato\n", *tid);
            pthread_mutex_unlock(&mutex);


        }

pthread_exit(NULL);



}


int main(int argc, char* argv[])
{
    pthread_t threads[SIZE];
    int tids[SIZE];
    sem_init(&aspetta, 0, SIZE);

    for(int i = 0; i< SIZE; i++)
        {
            printf("Inserisci %d-esimo numero tra 5 e 20: \n", i);
            scanf("%d", &array[i]);
        }

    for(int i = 0; i< SIZE; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], NULL, routine, (void *) &tids[i]);
    }

    for(int i = 0; i< SIZE; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&aspetta);

    exit(0);


}