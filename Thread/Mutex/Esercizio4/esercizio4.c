#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

#define SIZE 20


typedef struct 
{
    int buffer[SIZE];
    int count;
    int writepos;
    int readpos; 
}data; 

data global_struct;

int notfull();


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* productor(void * arg)
{
    data *buff = (data *) arg;

    for(int i = 0; i < 20; i++)
    {   
        while(notfull())
            pthread_cond_wait(&cond, &mutex);
        pthread_mutex_lock(&mutex);
        buff->count++;
        buff->buffer[buff->writepos] = i;
        buff->writepos++;
    }
}

void* consumer(data glob)
{
    
}


int notfull()
{
    for(int i = 0; i<SIZE; i++)
    {    
        if(global_struct.buffer[i] == -1){
            return 0;
        }
    }
    return 1;
}