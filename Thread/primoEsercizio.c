/* crea 5 thread a cui vengono passato due parametri tramite struct due campi id e messaggio */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct campi
{
    /* data */
    int tid;
    char message;
};

void *printiamo(void* campi){
    struct campi *params = (struct campi *) campi;
    printf("%s", params->message);
    printf("%d", params->tid);
}

int main(int argc, char* argv){

    pthread_t th[5];
    int count;
    struct campi *miastruct[5];


    for(int i = 0; i<5; i++){
        
        miastruct[i] = malloc((struct campi*) sizeof(struct campi));
        
        miastruct[i]->message="Ciao sono un thread";
        miastruct[i]->tid = i;
        
        if(pthread_create(&th[i], NULL, &printiamo, (void*) miastruct)!=0){
            return i;
        }

    }
    pthread_exit(NULL);
}
