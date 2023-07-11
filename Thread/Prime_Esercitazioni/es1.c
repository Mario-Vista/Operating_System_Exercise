
/*Facendo uso della libreria Pthread, realizzare un programma in cui un thread scrittore,
 dato un intero N da riga di comando (dove10<N<=15), scrive in un file nella prima posizione,
 uno alla volta edogni ½ secondo, la sequenza di Fibonacci di ordine N, alternandosi con un
 thread lettore che legge, uno alla volta dalla prima posizione del file, i numeri scritti dal
 thread scrittore.
  Un terzo thread attende la lettura dell’ N-esimo intero, quindi stampa a video
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
    int count;
    int fd;
    sem_t *leggi;
    sem_t *scrivi;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

} shared;

void init_struct();
void *writ(void *arg);
void *reade(void *arg);
void *stampa(void *arg);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("Error in input parameter");
        exit(0);
    }

    shared.N = atoi(argv[1]);

    init_struct();

    shared.fd = open("output.txt", 0, O_RDWR | O_CREAT | O_TRUNC, 0777);

    pthread_t writer, reader, stampatore;

    pthread_create(&writer, NULL, writ, NULL);
    pthread_create(&reader, NULL, reade, NULL);
    pthread_create(&stampatore, NULL, stampa, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);
    pthread_join(stampatore, NULL);

    sem_close(shared.leggi);
    sem_close(shared.scrivi);

    sem_unlink(LEGGI);
    sem_unlink(SCRIVI);

    pthread_cond_destroy(&shared.cond);
    pthread_mutex_destroy(&shared.mutex);

    exit(1);
}

void init_struct()
{
    sem_unlink(LEGGI);
    sem_unlink(SCRIVI);
    shared.count = 0;

    shared.leggi = sem_open(LEGGI, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
    shared.scrivi = sem_open(SCRIVI, O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
    pthread_mutex_init(&shared.mutex, NULL);
    pthread_cond_init(&shared.cond, NULL);
}

void *writ(void *arg)
{
    usleep(500000);
    int fib = 0;
    int a = 0;
    int b = 1;

    for (int i = 0; i < shared.N; i++)
    {
        sem_wait(shared.scrivi);
        fib = a + b;
        a = b;
        b = fib;

        fseek(shared.fd, 0, SEEK_SET);
        fprintf(shared.fd, "%d ", fib);
        sem_post(shared.leggi);
    }

    pthread_exit(NULL);
}

void *reade(void *arg)
{
    int buff;
    while (1)
    {
        sem_wait(shared.leggi);
        lseek(shared.fd, 0, SEEK_SET);
        read(shared.fd, &buff, sizeof(int));
        printf("%d\n", buff);
        pthread_mutex_lock(&shared.mutex);

        if (shared.count == shared.N)
        {
            pthread_cond_signal(&shared.cond);
            pthread_mutex_unlock(&shared.mutex);
            pthread_exit(NULL);
        }
        pthread_mutex_unlock(&shared.mutex);
        sem_post(shared.scrivi);
    }
        pthread_exit(NULL);

}

void* stampa(void *arg)
{
    pthread_mutex_lock(&shared.mutex);
    while(shared.count < shared.N)
    {
        pthread_cond_wait(&shared.cond, &shared.mutex);
    }
    pthread_mutex_unlock(&shared.mutex);

    printf("Operazioni concluse, arrivederci dal thread: tid");
    sleep(5);
    pthread_exit(NULL);
}
