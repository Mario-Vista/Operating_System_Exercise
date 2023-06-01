// Si realizzi un programma in C e Posix sotto Linux che,
// utilizzando la libreria Pthread
// • lancia n thread per cercare un elemento in una matrice nxn di
//  caratteri
// • Ognuno dei thread cerca l'elemento in una delle righe della
//  matrice
// • Non appena un thread ha trovato l'elemento cercato, rende note
//  agli altri thread le coordinate dell'elemento e tutti i thread
//  terminano (sono cancellati)

//mappa di memorizzazione = i*numero di colonne + j

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>

int **matrix;
int numSearch;
pthread_t *th;
bool flag;
int winner;

int size = 5;
int row, cols;
void create_matrix(int);
void print_matrix(int);

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void *args)
{
    int tids = *(int *) args;
    int j, k;

    for(j = 0; j < 5; j++)
    {
        if(matrix[tids][j] == numSearch)
        {  
                pthread_mutex_lock(&mutex);
                row = tids;
                cols = j;
                flag = true;
                winner = tids;
                pthread_mutex_unlock(&mutex);
                pthread_exit(NULL);
        }
    }
        pthread_exit(NULL);

}


int main(int argc, char *argv[]){
    srand(time(NULL));
    int *tids;

    create_matrix(size);
    print_matrix(size);

    printf("Quale numero vuoi cercare? ");
    scanf("%d", &numSearch);

    th = calloc(size, sizeof(pthread_t));
    tids = calloc(size, sizeof(int));

    for(int i = 0; i<size;i++)
    {
        tids[i] = i;
        if(pthread_create(&th[i], NULL, routine, (void *) &tids[i]) != 0)
        {
            perror("Error while creating thread");
            exit(0);
        }

    }

    for(int i = 0; i<size; i++)
    {
        if(flag == true)
        {
            if(tids[i] != winner)
            {
                pthread_cancel(th[i]);
            }
        }
        
        if(pthread_join(th[i], NULL) != 0)
        {
            perror("Error while joining thread");
            exit(1);
        }
    }

    printf("Elemento trovato alle coordinate %d, %d\n", row, cols);

    exit(1);
}


void create_matrix(int size)
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

void print_matrix(int size)
{
    for(int i = 0; i<size; i++)
    {
        for(int j = 0; j<size; j++)
        {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
}