/*Esercitazione 1

Facendo uso della libreria Pthread, realizzare un programma in cui un thread scrittore, dato un intero N 
da riga di comando (dove10<N<=15), scrive in un file nella prima posizione, uno alla volta edogni ½ secondo,
a sequenza di Fibonacci di ordine N, alternandosi conun thread lettore che legge, uno alla volta dalla prima
posizione del file, i numeri scritti dal thread scrittore. Un terzo thread attende lalettura dell’ N-esimo 
intero, quindi stampa a video il messaggio “Operazioni concluse, arrivederci dal thread: tid”, attende 5 secondi
e termina.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	sem_t leggi, scrivi;

}shared;


//variabili globali
int number;
int fd;
char filename[100];



shared *data;
void *init_struct();
void *leggi();
void *scrivi();
void *termini();

void *scrivi()
{
	//inizializzazioni variabili per la funzione di fibonacci
	int a = 0;
	int b = 1;
	int fib;

	printf("SOno il thread scrittore e funziono \n");	
	//ciclo  che va da 0 al numero inserito da tastiera
	for(int i = 0; i<number; i++)
	{
		//aspetto il semaforo per il turno in scrittura
		sem_wait(&data->scrivi);

		//aggiorno il numero di fibonacci
		fib = a + b;
		a = b;
		b = fib;

		//vado alla prima posizione del file
		fseek(fd, 0, SEEK_SET);

		//scrivo il numero i-esimo di fibonacci
		fprintf(fd, "%d\n", &fib);
		
		//faccio una signal sul semaforo in lettura
		sem_post(leggi);
	}

}


void *leggi()
{



}

int main(int argc, char *argv[])
{
	//numero
	number = 12;

	pthread_t lettore,scrittore,terminatore;



	//chiamo la funzione  che inizializza i campi della struct
	init_struct();

	fd = fopen(filename, "rw");
	if(fd ==-1)
	{
		perror("coglione");
		exit(0);
	}

	pthread_create(&scrittore,  NULL, scrivi, NULL);
	pthread_create(&lettore,  NULL, leggi, NULL);

	pthread_join(scrittore, NULL);
	pthread_join(lettore, NULL);


	//dealloco i semafori, il mutex e le variabili di condizione
	sem_destroy(&data->leggi);
	sem_destroy(&data->scrivi);

	pthread_mutex_destroy(&data->mutex);
	pthread_cond_destroy(&data->cond);

	exit(0);
}










void *init_struct()
{
	data = malloc(sizeof(shared));
	pthread_mutex_init(&data->mutex, NULL);
	pthread_cond_init(&data->cond, NULL);

	sem_init(&data->leggi,0,0);
	sem_init(&data->scrivi,0,1);
}