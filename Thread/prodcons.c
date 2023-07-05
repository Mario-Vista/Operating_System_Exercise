#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>


#define NBUFF 10
#define MUTEX "/mutex"
#define NEMPTY "/nempty"
#define NSTORED "/nstored"


int nitems;

typedef struct
{
	int buff[NBUFF];
	sem_t *mutex;	//=1
	sem_t *nempty;	//=NBUFF
	sem_t *nstored;	//=0
}shared;

shared *pointer;

//intestazione function
void *produce(void *);
void *consume(void *);
void init_struct();

int main(int argc, char *argv[])
{
	pthread_t producer, consumer;
	
	
	if(argc != 2)
	{
		printf("Error in input parameters");
		exit(-1);
	}

	nitems = atoi(argv[1]);
	
	
	init_struct();
	
	pthread_create(&producer, NULL, produce, NULL);
	pthread_create(&consumer, NULL, consume, NULL);
	
	pthread_join(producer, NULL);
	pthread_join(consumer, NULL);
	


	sem_close(pointer->mutex);
	sem_close(pointer->nempty);
	sem_close(pointer->nstored);

	sem_unlink(MUTEX);
	sem_unlink(NEMPTY);
	sem_unlink(NSTORED);


	exit(0);
	
}

void init_struct()
{	
	pointer = malloc(sizeof(shared));
	
	sem_unlink(MUTEX);
	sem_unlink(NEMPTY);
	sem_unlink(NSTORED);
	
		
	pointer->mutex = sem_open(MUTEX, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1);
	pointer->nempty = sem_open(NEMPTY, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR,NBUFF);
	pointer->nstored = sem_open(NSTORED, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 0);
	
}


void *produce(void *arg)
{
	printf("Produttore\n");
	for(int i = 0; i < nitems; i++)
	{
		sem_wait(pointer->nempty);

		sem_wait(pointer->mutex);
			pointer->buff[i%NBUFF] = i;
		sem_post(pointer->mutex);
		
		sem_post(pointer->nstored);
	}	
	pthread_exit(NULL);	
}

void *consume(void *arg)
{
	printf("Consumatore\n");

	for(int i = 0; i<nitems; i++)
	{
		sem_wait(pointer->nstored);
		sem_wait(pointer->mutex);
		if(pointer->buff[i%NBUFF] != i)
		{
			printf("buff[%d] = %d\n", i, pointer->buff[i % NBUFF]);
		}
		sem_post(pointer->mutex);
		sem_post(pointer->nempty);
	}
	pthread_exit(NULL);	
}