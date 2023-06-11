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

int number;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t *scrivi,*leggi;
int fd;


void *writ(void *data)
{
	ThreadParams *params = (ThreadParams *) data;
	int a =0;
	int b = 1;
	int fib;

	for(int i = 0; i<number; i++)
	{
		sem_wait(scrivi);
		fib = a + b;
		a = b;
		b = fib;

		fseek(params->file, 0, SEEK_SET);
		fprintf(params->file, "%s\n", fib);
		
		sem_post(leggi);
	}
}

void *red(void *)
{
	int i;
	int buff;
	while(1){
		sem_wait(sem2);
		lseek(fd,0,SEEK_SET);
		read(fd,&buff,sizeof(int));
		printf("Ho letto: %d\n",buff);
		sem_post(sem1);
		pthread_mutex_lock(&mutex);
		cnt++;
		if ( cnt == n ){
			pthread_cond_signal(&cond);
			pthread_mutex_unlock(&mutex);
			return;
		}
		pthread_mutex_unlock(&mutex);
	}
}


int main(int argc, char const *argv[])
{	

	sem_unlink("/leggi"); sem_unlink("/scrivi");
	leggi = sem_open("/leggi",O_CREAT,0777,1);
	scrivi = sem_open("/scrivi",O_CREAT,0777,0);

	fd = open("output.txt", 0, O_RDWR|O_CREAT|O_TRUNC, 0777);
	pthread_t writer, printer;

	if(file == NULL)
	{
		perror("Error while opening file");
		exit(1);
	}

	ThreadParams params;
	params.file = file;


	for(int i = 0; i < number; i++){
		pthread_create(&writer, NULL, writ, (void *) &params);
		
	}
	

	return 0;
}