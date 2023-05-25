//creazione di thread tramite un ciclo

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
int mails = 0;

void *routine(){

    for(int i = 0; i<100000; i++){
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }

}


int main(int argc, char* argv[]){

    pthread_t th[4];
    //inizializzo dinamicamente il mutex
    pthread_mutex_init(&mutex, NULL);

/* si usano due for (uno per la create e uno per la join) perchè in caso contrario i thread agirebbero uno alla volta */
    for(int i = 0; i<4; i++){
        if(pthread_create(&th[i], NULL, &routine, NULL) != 0){
            perror("Failed to create thread");
            return 1;
        }
        printf("Thread %d has started\n", i);

    }

    for(int i = 0; i<4; i++){

        if(pthread_join(th[i], NULL) != 0){
            return 2;
        }
        printf("Thread %d has finished\n", i);

    }

    //dealloco il mutex
    pthread_mutex_destroy(&mutex);

    printf("%d\n",  mails);
    return 0;
}