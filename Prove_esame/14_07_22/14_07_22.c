/**
 * ESAME: Prova Scritta 14/07/2022
*/

/**
 * 9 punti
 * 
 * Si realizzi un programma in C e Posix sotto linux che, con l'ausilio della libreria Pthread,
 * lancia m thread per calcolare la somma dei prodotti delle colonne di due matrici mxm.
 * 
 * L'i-esimo thread, dopo aver calcolato la somma dei prodotti delle colonne i-esime delle due matrici,
 * inserisce il risultato in un array m-dimensionale, in modo concorrente, nella prima locazione libera disponibile.
 * 
 * Non appena l'array sarà riempito, un m+1-esimo thread, che era in attesa, provvederà a stamparne il contenuto.
 * 
 * Le matrici devono essere allocate dinamicamente.
 * 
 * Usare mutex e variabili di condizione.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAXVAL 20


int size;

typedef struct
{
	int *result;
	int cont;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}data;

int **matrixOne;	
int **matrixTwo;
data *shared;


void *sum_cols(void * data)
{
	int * tid = (int *) data;
	int par = 0;
	
	for(int j = 0; j<size; j++)
	{

		par += matrixOne[*tid][j] * matrixTwo[*tid][j];
		
	}
	
	pthread_mutex_lock(&shared->mutex);
	if(shared->cont >= size)
	{
		pthread_cond_signal(&shared->cond);

	} else 
		{
			shared->cont++;
			shared->result[shared->cont] = par;
		}
	
	pthread_mutex_unlock(&shared->mutex);
	pthread_exit(NULL);
}

void *alert()
{
    pthread_mutex_lock(&shared->mutex);
	while(shared->cont < size)
	{
		pthread_cond_wait(&shared->cond, &shared->mutex);
	}
    pthread_mutex_unlock(&shared->mutex);


	printf("Risultati:\n");
	for(int i = 0; i<size; i++)
	{
		printf("%d\n", shared->result[i]);
	}
		
	pthread_exit(NULL);
}
	
void init_struct()
{

	shared = malloc(sizeof(data));
	shared->result = calloc(size, sizeof(int));
	shared->cont = 0;
	pthread_mutex_init(&shared->mutex, NULL);
	pthread_cond_init(&shared->cond, NULL);
	
}


void init_matrix()
{
	matrixOne = calloc(size, sizeof(int *));
	matrixTwo = calloc(size, sizeof(int *));
	
	for(int i = 0; i<size; i++)
	{
		matrixOne[i] = (int **) calloc(size, sizeof(int));
		matrixTwo[i] = (int **) calloc(size, sizeof(int));
	}
	
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
		matrixOne[i][j] = rand()%MAXVAL+1;
		matrixTwo[i][j] = rand()%MAXVAL+1;
		}
	}
	
}

void print_matrix()
{
	printf("First matrix \n");
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			printf("%d	", matrixOne[i][j]);
		}
		printf("\n");
	}
	
	
	printf("Second matrix \n");
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			printf("%d	", matrixTwo[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char* argv[])
{	
	size = atoi(argv[1]);
	pthread_t threads[size], printer;
	int tid[size];
	
	init_matrix();
	init_struct();
	
	print_matrix();
	
	for(int i = 0;  i<size; i++)
	{
		tid[i] = i;
		if(pthread_create(&threads[i], NULL, sum_cols, (void *) &tid[i]) != 0)
		{
			perror("Error while creating threads");
			exit(1);
		}
		
	}
	
    	if(pthread_create(&printer, NULL, alert, NULL) != 0)
		{
			perror("Error while creating threads");
			exit(1);
		}	
	
		
	for(int i = 0;  i<size; i++)
	{
		if(pthread_join(threads[i], NULL) != 0)
		{
			perror("Error while joining threads");
			exit(1);
		}
		
	}
	

		
		if(pthread_join(printer, NULL) != 0)
		{
			perror("Error while joining threads");
			exit(1);
		}
	
	exit(0);
}



