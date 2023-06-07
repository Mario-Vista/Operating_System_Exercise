#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>


int size;			//dimensione della matrice
int **matrix;			//matrice dinamica

//intestazione funzioni utilizzate
void init_matrix();
void print_matrix();
void init_struct();


//struttura dati per le informazioni usate dai thread
typedef struct data
{
	int *vec;
	int count;

}data;

//puntatore globale alla struttura data
data *info;


//struttura per mutex e variabile di condizione usati entrambi staticamente
struct
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
} shared = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};



//routine degli n thread, l'i-esimo thread lavora sull'i-esima riga della matrice ed estrae un numero casuale e lo inserisce nell'array di dimensione size/2
void * extract(void * data)
{
	//cast per passare il tid del thread
	int *tid = (int *) data;
	
	//genero il valore della colonna da cui dovrò estrarre il valore
	int cas= rand()%size+1;
	
	//entro in sezione critica
	pthread_mutex_lock(&shared.mutex);
	
	//controllo se l'array è pieno
	if(info->count >= size/2)
	{
		//se si mando un segnale al thread in attesa
		pthread_cond_signal(&shared.cond);
		
	} else
		{ 
			//altrimenti inserisco nel thread il numero selezionato
			info->vec[info->count] =  matrix[*tid][cas];
			info->count++;
		}
		
	//esco dalla sezione critica
	pthread_mutex_unlock(&shared.mutex);
	
	//termino il thread
	pthread_exit(NULL);
	
}

//thread addetto alla stampa del vettore risultato
void * wait()
{

	//entro in sezione critica
	pthread_mutex_lock(&shared.mutex);
	
	//finchè l'array risultato non è pieno
	while(info->count < size/2)
	{
		//aspetto un segnale
		pthread_cond_wait(&shared.cond, &shared.mutex);
	}
	
	//esco dalla sezione critica
	pthread_mutex_unlock;

	//stampo il vettore risultato
	printf("\nRisultati: \n");
	for(int i = 0; i<size/2; i++)
	{
		printf("%d\n", info->vec[i]);
	}
	pthread_exit(NULL);
}




int main()
{
	//prendo in input la dimensione della matrice
	printf("Inserisci la dimensione della matrice(deve essere pari): ");
	scanf("%d", &size);
	
	
	//dichiarazione delle variabili
	pthread_t threads[size],  printer;
	int tid[size];
	srand(time(NULL));

	//controllo se il size della matrice è pari	
	if(size %2 != 0){
		size++;
		printf("Il numero da te inserito è dispari quindi lo abbiamo incrementato di uno, ora è: %d\n", size);
	}
	
	//inizializzo la struttura, la matrice e stampo quest'ultima
	init_matrix();
	init_struct();
	print_matrix();
	
	//creo i thread che estraggono i numeri dalla matrice
	for(int i = 0; i<size; i++)
	{
		tid[i] = i;
		if(pthread_create(&threads[i], NULL, extract, (void *) &tid[i]) != 0)
		{
			perror("Error while creating thread\n");
		}
	}
	
	//creo il thread addetto alla stampa
	if(pthread_create(&printer, NULL, wait, NULL) != 0)
	{
		perror("Error while creating thread\n");
	}
	
	//joino i thread che estraggono i numeri dalla matrice
	for(int i = 0; i<size; i++)
	{
		if(pthread_join(threads[i], NULL) != 0)
		{
			perror("Error while joining thread\n");
		}	
	}
	
	//joino il thread addetto alla stampa
	if(pthread_join(printer, NULL) != 0)
	{
		perror("Error while joining thread\n");
	}

	//termina il programma
	exit(0);
}














//FUNCTION

//funzione per allocazione e assegnazione dei valori della matrice
void init_matrix()
{
	matrix =  (int **) calloc(size, sizeof(int *));
	
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


//funzione per la stampa della matrice
void print_matrix()
{
	printf("\nMatrice:\n");	
	for(int i = 0; i<size; i++)
	{
		for(int j = 0; j<size; j++)
		{
			printf("%d	", matrix[i][j]);
		}
		
	printf("\n");
	
	}
}


//allocazione della struct data e inizializzazione dei suoi cami
void init_struct()
{
	info = malloc(sizeof(data));
	info->vec = calloc((size+1)/2, sizeof(int));
	info->count = 0;
}


