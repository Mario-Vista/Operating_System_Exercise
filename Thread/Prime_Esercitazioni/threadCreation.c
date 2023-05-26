#include "../../../src.3e/apue.3e/include/apue.h"
#include <pthread.h>

pthread_t ntid;

void printids(const char *s)
{
    pid_t       pid;

    //gli id dei thread sono puntatori, anche se rappresentati da interi senza segno
    pthread_t   tid;
    
    pid = getpid();

    //il thread legge il suo id tramite questa function
    tid = pthread_self();

    printf("%s pid %d tid %lu (0x%lx)\n", s, (pid_t) pid, (unsigned long) tid, (unsigned long) tid );
}

void * thr_fn(void *arg){
    printids("nuovo thread: ");
    return ((void *) 0);
}

int main(void){
    int err;

    //pthread_create restituisce l'id del nuovo thread tramite il primo parametro
    err = pthread_create(&ntid, NULL, thr_fn, NULL);

    if(err != 0){
        fprintf(stderr, "non posso creare il thread: %s \n", strerror(err));
        exit(1);
    }

    printids("thread principale: ");

    //la sleep serve per evitare che il thread principale esca prima dell'esecuzione del nuovo thread
    //questo comportamento dipende dall'implementazione dei thread del sistema operativo e degli algoritmi di scheduling

    sleep(1);
    exit(0);
}