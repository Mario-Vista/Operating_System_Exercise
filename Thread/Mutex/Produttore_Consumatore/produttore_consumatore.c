/* Scrivere un programma C e Posix sotto Linux che, utilizzando la libreria Pthread crei due Thread,
 chiamati produttore e consumatore. La risorsa condivisa, è un buffer circolare di dimensione data
(ad esempio 20) il cui stato è :
• numero di elemento contenuti: count
• puntatore alla prima posizione libera: writepos
• puntatore al primo elemento occupato: readpos
II produttore, inserisce, 20 numeri interi in maniera sequenziale. II consumatore li estrae
 sequenzialmente per stamparli. Il programma dovrà prevedere:
• un meccanismo di accesso controllato alla risorsa buffer da parte dei due Thread 
(mutex per il controllo della mutua esclusione nell' accesso al buffer)
• una sincronizzazione tra il produttore ed il consumatore (Thread) in caso di
• buffer pieno: definizione di una condition per la sospensione del produttore se
 il buffer è pieno (notfull)
• buffer vuoto: definizione di una condition per la sospensione del consumatore se
 il buffer è vuoto (notempty)*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//definisco la dimensione del buffer
#define BUFFERSIZE 20

//struttura per i campi usati dal produttore e dal consumatore
typedef struct
{
    int buffer[BUFFERSIZE];
    int count;
    int writepos;
    int readpos;
    pthread_cond_t notfull;
    pthread_cond_t notempty;
    pthread_mutex_t mutex;

}data;

//puntatore globale alla struttura
data *buffer;

//funzione per l'inizializzazione dei thread
void *init_struct()
{
    //alloco la struttura
    buffer = malloc(sizeof(data));

    //inizializzo le variabili a 0
    buffer->count = 0;
    buffer->readpos = 0;
    buffer->writepos = 0;

    //inizializzo dinamicamente mutex e variabili di condizione
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->notempty, NULL);
    pthread_cond_init(&buffer->notfull, NULL);

}

//funzione per estrarre un elemento dal buffer
int estrai()
{
    //variavile per il valore estratto dal buffer
    int retval;

    //entro in sezione critica
    pthread_mutex_lock(&buffer->mutex);

    //finchè non ci sono elementi nel buffer(vuoto)
    while(buffer->count == 0)
    {   
        //aspetto di essere attivato
        pthread_cond_wait(&buffer->notempty, &buffer->mutex);
    }

    //estraggo il valore dalla prima posizione piena nel buffer
    retval = buffer->buffer[buffer->readpos];

    //incremento l'indice della posizione da cui estrarre l'elemento
    buffer->readpos++;

    //decremento il contatore di elementi presenti nel buffer
    buffer->count--;
    
    //se l'indice di posizione per estrarre è alla fine del buffer riparte da 0(poichè circolare)
    if(buffer->readpos >= BUFFERSIZE)
    {
        buffer->readpos = 0;
    }
    //segnalo che c'è almeno una posizione vuota(dovuta dall'estrazione dell'elemento fatta in precedenza)
    pthread_cond_signal(&buffer->notfull);

    //esco dalla sezione critica
    pthread_mutex_unlock(&buffer->mutex);

    //ritorno il valore estratto
    return retval;
    
}

//funzione per inserire un elemento nel buffer
//prende in input il numero da inserire nel buffer
void inserisci(int number)
{
    //entro in sezione critica
    pthread_mutex_lock(&buffer->mutex);

    //finchè il buffer è pieno aspetto
    while(buffer->count == BUFFERSIZE)
    {
        pthread_cond_wait(&buffer->notfull, &buffer->mutex);
    }

    //inserisco nella posizione indicata da writepos il numero preso in input
    buffer->buffer[buffer->writepos] = number;

    //incremento il puntatore alla posizione successiva
    buffer->writepos++;

    //incremento il contatore di elementi presenti nel buffer
    buffer->count++;

    //se il puntatore è alla fine del buffer riparte da capo
    if(buffer->writepos >= BUFFERSIZE)
    {
        buffer->writepos = 0;
    }

    //segnalo che c'è almeno una posizione vuota nel buffer
    pthread_cond_signal(&buffer->notempty);
    
    //esco dalla mutua esclusione
    pthread_mutex_unlock(&buffer->mutex);

}


//routine del produttore
void *producer(void * args)
{
    printf("Sono il produttore e produco...\n");
    
    //produce 20 interi nel buffer
    for(int i = 0; i<20; i++)
    {
        inserisci(i);
    }
    pthread_exit(NULL);

}

//routine del consumatore
void *consumer(void * args)
{
    int stamp;
    printf("sono il consumatore e consumo...\n");

    for(int i = 0; i<20; i++){
        //estrae e stampa a schermo gli elementi presenti nel buffer
        stamp = estrai();
        printf("%d\n", stamp);
    }
    pthread_exit(NULL);

}

int main(int argc, char* argv[])
{
    //dichiaro i due thread
    pthread_t prod, cons;

    //richiamo la funzione per allocare e inizializzare tutti i campi della struct
    init_struct();

    //creo il thread produttore
    if(pthread_create(&prod, NULL, producer, NULL) != 0)
    {
        perror("Error while creating producer thread");
        exit(1);
    }

     //creo il thread consumatore
    if(pthread_create(&cons, NULL, consumer, NULL) != 0)
    {
        perror("Error while creating consumer thread");
        exit(1);
    }

    //effettuo la join del thread produttore
    if(pthread_join(prod, NULL) != 0)
    {
        perror("Error while joining producer thread");
        exit(1);
    }

    //effettuo la join del thread consumatore
    if(pthread_join(cons, NULL) != 0)
    {
        perror("Error while joining consumer thread");
        exit(1);
    }

    //dealloco thread e variabili di condizione
    pthread_cond_destroy(&buffer->notempty);
    pthread_cond_destroy(&buffer->notfull);
    pthread_mutex_destroy(&buffer->mutex);

}