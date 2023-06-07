/**
 * Si realizzi un programma in C e Posix sotto Linux che, con l’ausilio della libreria Pthread, 
 * lancia 3 thread che leggono concorrentemente altrettante directory specificate dall'utente.  
 * I thread, inoltre,scrivono in  una variabile il numero di file della directory, tra le tre esaminate
 * contenente il minor numero di entrate. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int minimum = 999999999;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *routine(void * data)
{
	char *directory = (char *) data;
	
	DIR *dir;
	struct dirent *entry;
	int count = 0;
	
	dir = opendir(directory);
	if(dir == NULL)
	{
		perror("Impossibile aprire la directory");
		pthread_exit(NULL);
	}
	
	while((entry = readdir(dir)) != NULL)
	{
		if(strcmp(entry->d_name,".") != 0 && strcmp(entry->d_name,"..") != 0)
		{
			count++;
		}
	}
	
	closedir(dir);
	
	pthread_mutex_lock(&mutex);
	if(count < minimum)
	{
		minimum = count;
	}
	pthread_mutex_unlock(&mutex);
}



int main()
{
	char directoryOne[100], directoryTwo[100], directoryThree[100];
	pthread_t th[3];
	
	printf("Inserire tre directory\n");
	scanf("%s", directoryOne);
	scanf("%s", directoryTwo);
	scanf("%s", directoryThree);
	
	pthread_create(&th[0], NULL, routine, (void*) &directoryOne);	
	
	pthread_create(&th[1], NULL, routine, (void*) &directoryTwo);	
	
	pthread_create(&th[2], NULL, routine, (void*) &directoryThree);		
		
	
	pthread_join(th[0], NULL);
	pthread_join(th[1], NULL);
	pthread_join(th[2], NULL);

	
	printf("min %d\n", minimum);
}
