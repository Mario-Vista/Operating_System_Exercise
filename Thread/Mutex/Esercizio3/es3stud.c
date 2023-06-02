/* Scrivere un programma C e Posix sotto Linux che, utilizzando la libreria
Pthread crei due Thread "somma1" e "somma2",entrambi accedono alle variabili
test.a e test.b di una struttura dati test condivisa incrementandole di 1 per 10
volte. Si utilizzi come meccanismo di
gestione della concorrenza quello della "mutua esclusione per struttura",
dove la struttura mutex va allocata dinamicamente*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#define LOOP 10

typedef struct ciao{
    int a;
    int b;
    pthread_mutex_t mutex;
}data;



data *test;

data *init_struct()
{
    data *miastruct = malloc(sizeof(data));
    miastruct->a = 0;
    miastruct->b = 0;

    pthread_mutex_init(&miastruct->mutex, NULL);
    return (miastruct);
}


void *somma1(void* args)
{

    for(int i = 0; i<LOOP ; i++)

    {
        pthread_mutex_lock(&test->mutex);
        test->a++;
        test->b++;
        pthread_mutex_unlock(&test->mutex);

    }
        pthread_exit(NULL);

}

void* somma2(void * args)
{

    for(int i = 0; i<LOOP ; i++)
    {
        pthread_mutex_lock(&test->mutex);
        test->a ++;
        test->b ++;
        pthread_mutex_unlock(&test->mutex);

    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[])
{
    pthread_t t1, t2;

    test = init_struct();
   

    pthread_create(&t1, NULL, somma1, NULL);
    pthread_create(&t2, NULL, somma2, NULL);

        pthread_join(t1, NULL);

        pthread_join(t2, NULL);

    printf("test.a: %d\n", test->a);
    printf("test.a: %d\n", test->b);

    pthread_mutex_destroy(&test->mutex);

    exit(0);



}

