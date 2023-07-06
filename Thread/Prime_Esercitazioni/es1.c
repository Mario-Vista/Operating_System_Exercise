
/*Facendo uso della libreria Pthread, realizzare un programma in cui un thread scrittore,
 dato un intero N da riga di comando (dove10<N<=15), scrive in un file nella prima posizione, 
 uno alla volta edogni ½ secondo, la sequenza di Fibonacci di ordine N, alternandosi con un 
 thread lettore che legge, uno alla volta dalla prima posizione del file, i numeri scritti dal 
 thread scrittore. Un terzo thread attende la lettura dell’ N-esimo intero, quindi stampa a video 
 il messaggio“Operazioni concluse, arrivederci dal thread: tid”, attende 5 secondi etermina.*/


#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>


#define LEGGI "/leggi"
#define SCRIVI "/scrivi"

struct 
{
    /* data */
    int N;
    int fd;
    sem_t *leggi;
    sem_t *scrivi;

}shared;

void init_struct();
void *write(void *arg);
void *read(void *arg);



int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        perror("Error in input parameter");
        exit(0);
    }

    shared.N = atoi(argv[1]);

    init_struct();


    shared.fd = open("output.txt", 0, O_RDONLY | O_CREAT | O_TRUNC, 0777);
    
    pthread_t writer, reader;

    pthread_create(&writer, NULL, write, NULL); 
    pthread_create(&reader, NULL, read, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    exit(1);

}

void init_struct()
{
    sem_unlink(LEGGI);
    sem_unlink(SCRIVI);

    shared.leggi = sem_open(LEGGI, O_CREAT|O_EXCL, S_IRUSR | S_IRUSR, 0);
    shared.scrivi = sem_open(LEGGI, O_CREAT|O_EXCL, S_IRUSR | S_IRUSR, 1);


}


void *write(void *arg)
{
    usleep(500000);
    int fib = 0;
    int a = 0;
    int b = 1;

    for(int i = 0; i< shared.N; i++)
    {
        sem_wait(shared.scrivi);
        fib = a+b;
        a = b;
        b = fib;

        fseek(shared.fd, 0, SEEK_SET);
        fprintf(shared.fd, "%d ", fib);
        sem_post(shared.leggi);
    }

    pthread_exit(NULL);
}

void *reader(void *arg)
{
    while

}

