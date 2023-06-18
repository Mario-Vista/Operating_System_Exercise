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

//definisco il numero di thread
#define SIZE 5

//creo un array globale
int array[SIZE];

//variabile globale che contiene il valore del semaforo contatore <aspetta>
int val;

//semaforo contatore per sincronizzare la terminazione dei thread
sem_t aspetta;

//semaforo binario per la mutua esclusione
sem_t cs;


//routine eseguita dal thread
void *routine(void *data)
{
    //id del thread
    int *tid = (int *) data;

    //il thread dorme un numero di secondi pari al valore corrispondente dell'array
    sleep(array[*tid]);

    //entro in sezione critica
    sem_wait(&cs);
        
        printf("Thread #%d svegliato!\n", *tid);
        //incremento il semaforo contatore che gestisce la sincronizzazione dei thread
        sem_post(&aspetta);
        //aggiorno il valore del semaforo contenuto nell'apposita variabile
        sem_getvalue(&aspetta, &val);

    //esco dalla sezione critica
    sem_post(&cs);

    //finchè non sono stati creati tutti i thread
    while(val < SIZE)
    {
        //aspetta
    }

    printf("\n");

    printf("Thread #%d terminato!\n", *tid);

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    //dichiaro un numero di thread pari alla costante definita in precedenza
    pthread_t threads[SIZE];

    //creo un array di indici dei thread
    int tids[SIZE];
    
    //inizializzo i semafori per memoria
    sem_init(&aspetta, 0, 0);
    sem_init(&cs, 1, 1);

    //prendo in input i valori del thread
    for(int i = 0; i< SIZE; i++)
        {
            printf("Inserisci %d-esimo numero tra 5 e 20: \n", i);
            scanf("%d", &array[i]);

            //inserisci controllo
        }

    //creo i thread
    for(int i = 0; i< SIZE; i++)
    {
        tids[i] = i;
        pthread_create(&threads[i], NULL, routine, (void *) &tids[i]);
    }


    //aspetto la terminazione dei thread
    for(int i = 0; i< SIZE; i++)
    {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&aspetta);
    sem_destroy(&cs);

    exit(0);


}