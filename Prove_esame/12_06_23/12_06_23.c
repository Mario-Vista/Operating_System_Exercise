/*
Date due matrici A e B, di dimensioni mxn e nxm di interi generati casualmente, allocate dinamicamente, con m e n
argomenti da riga di comando, creare m thread che provvedono a calcolare il prodotto delle due matrici A e B, denotato con C,
e dove ciascun thrvoidead calcola il prodotto relativo alla sua riga di competenza. Un thread m+1-esimo, in attesa che
la matrice prodotto sia completa, viene risvegliato e stampa C.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

//dichiaro globalmente le dimensioni della matrice
int m,n;

//dichiaro e inizializzo(staticamente) un mutex e una variabile di condizione
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//dim mxn
int **matrixA;

//dim nxm
int **matrixB;

//dim mxm
int **matrixC;

//variabile per controllare se è terminato il calcolo
int count = 0;


//intestazione di function
void allocation();
void print_matrix(int **matrix, int row, int 	 ols);


//calcolo prodotto matrice per vettore
void *prodotto(void *data)
{
	//indice di riga su cui deve lavorare il singolo thread
	int *tid = (int *)data;

	//calcolo effettivo del prodotto
	for(int i = 0; i<m; i++)
	{
		for(int j = 0; j<n; j++)
		{
			matrixC[*tid][i]+=matrixA[*tid][j]*matrixB[j][i];
		}
	}

	//inizio della sezione critica
	pthread_mutex_lock(&mutex);
	
	//aggiorno il contatore di righe calcolate
	count++;
	
	//se sono state calcolate tutte
	if(count == m)
	{
		//sveglio l'm+1-esimo thread
		pthread_cond_signal(&cond);
	}

	//fine della sezione critica
	pthread_mutex_unlock(&mutex);
}

//thread addetto alla stampa
void *attesa()
{
	//entro in sezione critica
	pthread_mutex_lock(&mutex);

	//finchè non sono stati calcolati tutit gli elementi
	while(count < m)
	{
		//aspetto
		pthread_cond_wait(&cond, &mutex);
	}

	//esco dalla sezione critica
	pthread_mutex_unlock(&mutex);

	printf("Matrice Risultante\n");

	print_matrix(matrixC, m, m);

}



int main(int argc, char *argv[])
{
	srand(time(NULL));


	if(argc != 3)
	{
		perror("Error in input parameters");
		exit(-1);
	}

	//prendo le dimensioni in input da riga di comando
	m = atoi(argv[1]);
	n = atoi(argv[2]);

	//alloco la patrice
	allocation();

	printf("Matrice A\n");
	print_matrix(matrixA, m, n);

	printf("Matrice B\n");
	print_matrix(matrixB, n, m);

	//dichiaro i thread
	pthread_t threads[m], printer;

	//dichiaro un array di dimensione m per gli id dei thread
	int tid[m];

	//creo i thread per il calcolo del prodotto matrice per matrice
	for(int i = 0; i<m; i++)
	{
		tid[i] = i;
		if(pthread_create(&threads	[i], NULL, prodotto, &tid[i])!= 0)
		{
			perror("Create");
			exit(1);
		}
	}

	//creo il thread addetto alla stampa
	if(pthread_create(&printer, NULL, attesa, NULL)!= 0)
	{
		perror("Create");
		exit(1);
	}

	//joino i thread per il calcolo del prodotto matrice per matrice
	for(int i = 0; i<m; i++)
	{
		tid[i] = i;
		if(pthread_join(threads	[i], NULL)!= 0)
		{
			perror("JOIN");
			exit(1);
		}
	}
	
	//creo il thread addetto alla stampa
	if(pthread_join(printer, NULL)!= 0)
	{
		perror("JOIN");
		exit(1);
	}

	exit(0);
}


//funzione che alloca e riempie la matrice
void allocation()
{
	matrixA = malloc(m * sizeof(int *));
	matrixB = malloc(n*sizeof(int *));
	matrixC = calloc(m,sizeof(int *));

	for(int i = 0; i < m; i++ )
	{
		matrixA[i] = malloc(n * sizeof(int));
	}

	for(int i = 0; i < n; i++)
	{
		matrixB[i] = malloc(m * sizeof(int));
		matrixC[i] = calloc(m, sizeof(int));
	}

	for(int i = 0; i < m; i++)
	{
		for(int j = 0; j < n; j++)
		{
			matrixA[i][j] = rand()%11;
			matrixB[j][i] = rand()%11;
		}
	}
}


//funzione che stampa la matrice data in input la matrice e le dimensioni di righe e colonne
void print_matrix(int **matrix, int row, int cols)
{
	printf("\n Stampa \n");

	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < cols; j++)
		{
			printf("%d	",matrix[i][j]);
		}
	printf("\n");
	}
}

