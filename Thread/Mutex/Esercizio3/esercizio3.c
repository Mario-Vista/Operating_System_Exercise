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
#include <signal.h>

typedef struct
{
    int a;
    int b;
} test;

pthread_mutex_t mutex;

void *routine(void *argc)
{
    test *data = (test *)argc;

    for (int i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mutex);
        data->a++;
        data->b++;
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t somma[2];

    test *data;

    data = malloc(sizeof(test));

    data->a = 0;
    data->b = 0;
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < 2; i++)
    {
        pthread_create(&somma[i], NULL, routine, (void *)&data);
    }

    for (int i = 0; i < 2; i++)
    {
        pthread_join(somma[i], NULL);
    }

    printf("%d \n %d", data->a, data->b);

    pthread_mutex_destroy(&mutex);
}