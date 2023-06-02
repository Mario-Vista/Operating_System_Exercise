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
#define BUFFERSIZE 20


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

data *buffer;

void init_struct()
{
    buffer->count = 0;
    buffer->readpos = 0;
    buffer->writepos = 0;
    pthread_mutex_init(&buffer->mutex, NULL);
    pthread_cond_init(&buffer->notempty, NULL);
    pthread_cond_init(&buffer->notfull, NULL);

}



void *producer()
{
    

}

void *consumer()
{

}

int main(int argc, char* argv[])
{

}