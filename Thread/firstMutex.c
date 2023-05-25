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

    pthread_t t1, t2;
    //inizializzo dinamicamente il mutex
    pthread_mutex_init(&mutex, NULL);

    if(pthread_create(&t1, NULL, &routine, NULL) != 0){
        return 1;
    }

    if(pthread_create(&t2, NULL, &routine, NULL) != 0){
        return 2;
    }

    if(pthread_join(t1, NULL) != 0){
        return 3;
    }

    if(pthread_join(t2, NULL) != 0){
        return 4;
    }

    //dealloco il mutex
    pthread_mutex_destroy(&mutex);

    printf("%d\n",  mails);
    return 0;
}