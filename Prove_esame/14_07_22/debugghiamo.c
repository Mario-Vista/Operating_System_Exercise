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

//definisco il valore massimo che può essere assunto dagli elementi della matrice
#define MAXVAL 20

//variabile globale per la dimensione della matrice
int size;


//struct globale dei campi usati dal mutex e per la mutua esclusione
typedef struct
{
	int *result;			//array dei risultati
	int cont;			//contatore di elementi presenti nell'array di risultati
	pthread_mutex_t mutex;		//mutex
	pthread_cond_t cond;		//variabile di condizione
}data;

//matrici allocate globalmente
int **matrixOne;	
int **matrixTwo;

//puntatore globale per accedere alla struct
data *shared;


//function che alloca e assegna i valori alle due matrici dinamiche
void init_matrix()
{
	//alloco le righe delle matrici
	matrixOne = calloc(size, sizeof(int *));
	matrixTwo = calloc(size, sizeof(int *));
	
	//alloco le colonne delle matrici
	for(int i = 0; i<size; i++)
	{
		matrixOne[i] = (int **) calloc(size, sizeof(int));
		matrixTwo[i] = (int **) calloc(size, sizeof(int));
	}

	
	//assegno i valori alle celle delle matrici	
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
		matrixOne[i][j] = rand()%MAXVAL+1;
		matrixTwo[i][j] = rand()%MAXVAL+1;
		}
	}

}

//function per la stampa delle matrici
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

//alloco la struttura e inizializzo i suoi campi
void init_struct()
{

	shared = malloc(sizeof(data));
	shared->result = calloc(size, sizeof(int));
	shared->cont = 0;
	pthread_mutex_init(&shared->mutex, NULL);
	pthread_cond_init(&shared->cond, NULL);
	
}

//routine svolta dagli m thread per la somma dei prodotti tra gli elementi della i-esima colonna della matrice
void *sum_cols(void * data)
{
	//passo tramite il valore tid la riga sulla quale il thread deve lavorare
	int * tid = (int *) data;
	
	//somma parziale dei prodotti degli elementi della matrice
	int par = 0;
	
	//calcolo la somma parziale dell'i-esima colonna dei due thread
	for(int j = 0; j<size; j++)
	{
	
		par += matrixOne[*tid][j] * matrixTwo[*tid][j];
		
	}
	
	//entro in sezione critica
	pthread_mutex_lock(&shared->mutex);
	
	//controllo se sono l'ultimo thread ad inserire la somma nell'array di risultati
	if(shared->cont >= size)
	{
		//se si segnalo al thread che deve stampare che tutte le somme sono inserite nell'array di risultati
		pthread_cond_signal(&shared->cond);

	} else 
		{
			//altrimenti inserisco l'elemento e incremento il contatore;
			shared->result[shared->cont] = par;
			shared->cont++;
		}
	
	//esco dalla sezione critica
	pthread_mutex_unlock(&shared->mutex);
	
	//termino il thread
	pthread_exit(NULL);
}

//funzione del thread che aspetta la terminazione
void *alert()
{
	//entro in sezione critica
	pthread_mutex_lock(&shared->mutex);

	//finchè il buffer non è pieno
	while(shared->cont < size)
	{	
		//aspetto una signal
		pthread_cond_wait(&shared->cond, &shared->mutex);
	}
	
	//esco dalla sezione critica
	pthread_mutex_unlock(&shared->mutex);

	//stampo i risultati
	printf("Results:\n");
	for(int i = 0; i<size; i++)
	{
		printf("Sum of product number %d:	%d\n",i+1, shared->result[i]);
	}

	//termino il thread		
	pthread_exit(NULL);
}





//MAIN
int main(int argc, char* argv[])
{	
	//assegno alla size il valore preso da riga di comando
	size = atoi(argv[1]);
	
	//dichiaro una variabile tid che indica l'id del threaad
	int tid[size];
	
	//creo i thread per il prodotto della matrice e quello addetto alla stampa
	pthread_t threads[size], printer;


	//inizializzo la matrice e la struct richiamanto le apposite funzioni
	init_matrix();
	init_struct();
	
	//stampo la matrice
	print_matrix();
	
	//creo i thread addetti alla somma dei prodotti
	for(int i = 0;  i<size; i++)
	{
		tid[i] = i;

		if(pthread_create(&threads[i], NULL, sum_cols, (void*) &tid[i]) != 0)
		{
			perror("Error while creating threads");
			exit(1);
		}
		
	}

	//creo il thread addetto alla stampa
	if(pthread_create(&printer, NULL, alert, NULL) != 0)
	{
		perror("Error while creating threads");
		exit(1);
	}

	//joino i thread addetti alla somma dei prodotti
	for(int i = 0;  i<size; i++)
	{
		if(pthread_join(threads[i], NULL) != 0)
		{
			perror("Error while joining threads");
			exit(1);
		}
		
	}
	
	//joino il thread addetto alla stampa
	if(pthread_join(printer, NULL) != 0)
	{
		perror("Error while joining threads");
		exit(1);
	}
	
	
}
	
	
	
	
