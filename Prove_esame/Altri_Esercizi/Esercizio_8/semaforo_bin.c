#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const char *semaforo = "/semaphore";
sem_t *semaphore;

int main()
{
	char filename[100] = "ciao.txt";
	FILE *file;
	sem_unlink(semaforo);

	semaphore = sem_open(semaforo,O_CREAT|O_EXCL, S_IRUSR|S_IWUSR, 1);

	int val,val2;
	sem_getvalue(semaphore, &val);

	file = fopen("ciao.txt", "rw");

	fprintf(file, "%d\n", val);

    while (fscanf(file, "%d", &val2) != EOF)
    {

    }
   	printf("%d\n", val2);


	fclose(file);
	sem_close(semaphore);
	sem_unlink(semaforo);
}