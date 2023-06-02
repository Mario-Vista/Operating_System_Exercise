/*
Si realizzi un programma in C e Posix sotto Linux che, con l'ausilio della libreria Pthread
    Lancia n thread per calcolare la somma degli elementi di ciascuna riga di una matrice nxn di 
    interi generati casualmente in un intervallo [0,255], allocata dinamicamente

    Il calcolo della somma degli elementi di ciascuna riga deve essere effettuato concorrentemente su tutte
    le righe, secondo la seguente modalità
        Il thread i-esimo con i pari, calcola la somma degli elementi di indice pari della riga i-esima
        Il thread con indice i dispari, calcola la somma degli elementi di indice dispari della riga i-esima

    Calcolate le somme parziali, si provvederà a ricercarne il minimo e a stamparlo a video
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int **matrix;
int size = 5;
int partialSum[5];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int minimum = 9999999;



void create_matrix(int, int);
void print_matrix(int, int);

void *routine(void * data)
{
    //passo il tid come argomento
    int *tid = (int *) data;

    //se il tid è pari
    if(*tid%2 == 0)
    {   
        //calcola la somma parziale degli elementi pari
        for(int i = 0; i < size; i+2)
        {
            partialSum[*tid]+=matrix[*tid][i];
        }
        //se è dispari
    } else {
        //calcola la somma parziale degli elementi dispari
        for(int i = 1; i < size; i+2)
        {
            partialSum[*tid]+=matrix[*tid][i];
        }
    }

    //blocco il mutex
    pthread_mutex_lock(&mutex);

    //se la osmma parziale è minore del minimo
    if(partialSum[*tid] < minimum)
    {
        //aggiorno il valore del minimo
        minimum = partialSum[*tid];
    }
    //sblocco il mutex
    pthread_mutex_unlock(&mutex);
    //termino il  thread
    pthread_exit(NULL);
}



int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    size;


    pthread_t thread[size];
    int tids[size];

    create_matrix(size, size);


    for(int i = 0; i<size; i++)
    {
        tids[i] = i;
        if(pthread_create(&thread[i], NULL, routine, (void*) &tids[i]) != 0)
        {
            perror("Error while creating thread");
            exit(1);
        }
    }

    for(int i = 0; i < size; i++)
    {
        if(pthread_join(thread[i], NULL) != 0)
        {
            perror("Error while joining thread");
            exit(1);
        }
    }

    for(int i = 0; i<size; i++)
    {
        printf("Partial sum %d-esima: %d\n", i, partialSum[i]);
    }

    printf("Minimum: %d\n", minimum);

    
    
    return 0;
}



void create_matrix(int row, int cols)
{
    matrix = calloc(size, sizeof(int *));

    for(int i = 0; i<size; i++)
    {
        matrix[i] = calloc(size, sizeof(int));
    }

    for(int i = 0; i<size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            matrix[i][j] = rand()%21;
        }
    }
}

void print_matrix(int row, int cols)
{
    for(int i = 0; i<row; i++)
    {
        for(int j = 0; j<cols; j++)
        {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
}