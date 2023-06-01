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
#include <signal.h>

int **matrix;
int size;

typedef struct {
    int *partialSum;
    int tid
}data;

void create_matrix(int, int);
void print_matrix(int, int);

void * routine(void * argv){
    pthread_t *tid = (pthread_t *) argv;
    int parSum = 0;

    if (*tid%2 == 0)
    {
        for(int i = 0; i< size; i+2)
            parSum+=matrix[*tid][i];
    } else {
        for(int i = 1; i< size; i+2)
            parSum+=matrix[*tid][i];
    }
    

}


int main(int argc, char const *argv[])
{
    srand(time(NULL));
    
    size = atoi(argv[1]);

    pthread_t thread[size];


    
    
    return 0;
}



void create_matrix(int row, int cols){
    //malloc
    //matrix = (int*) malloc(row * cols * sizeof(int));
    //calloc
    matrix = (int **)calloc(row, sizeof(int *));
        
    for(int i = 0; i<row; i++){
        matrix[i] = (int*)calloc(cols, sizeof(int));

    
    }

    for(int i = 0; i<row; i++){
        for(int j = 0; j<cols; j++){
           matrix[i][j] = rand()%255+1;
        }

    }
}

void print_matrix(int row, int cols)
{
    for(int i = 0; i<row; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            printf("%d  ", matrix[i][j]);
        }
        printf("\n");
    }
}



