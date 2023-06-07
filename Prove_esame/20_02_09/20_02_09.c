/**
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread,  
 * crei una matrice di interi di dimensioni n x n con n numero dispari fornito da riga di comando,  
 * che provveda a sommare in modo concorrente con quattro thread: 
 *  1. gli elementi delle due diagonali;
 *  2. gli elementi della riga centrale;
 *  3. gli elementi della colonna centrale; 
 * e ne determini il massimo da assegnare ad un'opportuna variabile. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

int **matrix;
int size;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int max = -1;

void deallocaMatrice() {
    // Deallocazione delle righe della matrice
    for (int i = 0; i < size; i++) {
        free(matrix[i]);
    }

    // Deallocazione del puntatore alla matrice
    free(matrix);
}

void init_matrix()
{
	matrix = (int **) calloc(size, sizeof(int *));
	
	for(int i = 0; i<size; i++)
	{
		matrix[i] = (int *) calloc(size, sizeof(int));
	}
	
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			matrix[i][j] = rand()%21;
		}
	}
	
}


void *lefts()
{
	int par = 0;
	for(int i = 0; i<size; i++)
	{
		par+=matrix[i][i];	
	}
	
	pthread_mutex_lock(&mutex);
	if(par > max)
	{
		max = par;
	}
	pthread_mutex_unlock(&mutex);
	
	printf("\nleft: %d\n", par);
	
	pthread_exit(NULL);	
}

void *rights()
{

	int par = 0;
	for(int i = 0; i<size; i++)
	{
			par+=matrix[i][size - 1 - i];		
		
	}
	
		pthread_mutex_lock(&mutex);
	if(par > max)
	{
		max = par;
	}
	pthread_mutex_unlock(&mutex);	
	
	printf("\nright: %d\n", par);		

pthread_exit(NULL);
}

void *rows()
{
	int par = 0;
	int center = size/2;
	
	for(int i = 0; i<size; i++)
	{
		par+=matrix[center][i];	
		
	}
	
	pthread_mutex_lock(&mutex);
	if(par > max)
	{
		max = par;
	}
	pthread_mutex_unlock(&mutex);	
	
	printf("\nrow: %d\n", par);
	
	pthread_exit(NULL);	
}

void *colss()
{	
	int par = 0;
	int center = size/2;
	
	for(int i = 0; i<size; i++)
	{
		par+=matrix[i][center];	
		
	}
	
	pthread_mutex_lock(&mutex);
	if(par > max)
	{
		max = par;
	}
	pthread_mutex_unlock(&mutex);	
	
	printf("\ncols: %d\n", par);
	
	pthread_exit(NULL);
}


void print_matrix()
{
	printf("Matrice:\n");
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			printf("%d	",matrix[i][j]);
		}
	
		printf("\n");
	}
}


int main(int argc, char* argv[])
{
	srand(time(NULL));
	size = atoi(argv[1]);
	
	pthread_t left, right, row, cols;
	
	init_matrix();
	print_matrix();
	
	
	if(pthread_create(&left, NULL, lefts, NULL) != 0)
	{
		perror("Error while creating thread");
	}
	
	if(pthread_create(&right, NULL, rights, NULL) != 0)
	{
		perror("Error while creating thread");
	}
	
	if(pthread_create(&row, NULL, rows, NULL) != 0)
	{
		perror("Error while creating thread");
	}
	
	if(pthread_create(&cols, NULL, colss, NULL) != 0)
	{
		perror("Error while creating thread");
	}


	if(pthread_join(left, NULL) != 0)
	{
		perror("Error while joining thread");
	}
	
	if(pthread_join(right, NULL) != 0)
	{
		perror("Error while joining thread");
	}
	
	if(pthread_join(row, NULL) != 0)
	{
		perror("Error while joining thread");
	}
	
	if(pthread_join(cols, NULL) != 0)
	{
		perror("Error while joining thread");
	}
	
	printf("\nmax: %d\n", max);
	
	deallocaMatrice();
	exit(0);

}
