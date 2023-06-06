 /* Scrivere un programma in C che ( in ambiente Linux e utilizzando la libreria pthread ) crei 2 thread che eseguono
 * la funzione "incrementa" che a sua volta accede alle variabili glob.a e glob.b di una struttura dati condivisa glob e
 * ne incrementi il loro valore di 1 per 100 volte. Al termine, quando i due thread avranno terminato con gli 
 * incrementi, il thread principale stamperà a video i valori delle variabili test.a e test.b. Per la gestione della
 * sincronizzazione si utilizzi i mutex allocati dinamicamente.*/
 
 #include <stdio.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <pthread.h>
 #define ITERATION 100
 
 typedef struct {
 	int a;
 	int b;
 	pthread_mutex_t mutex;
 }glob;
 
 
 glob * pointer;
 
 
 void init_struct()
 {
 	pointer = malloc(sizeof(glob));
 	pointer->a = 0;
 	pointer->b = 0;
 	pthread_mutex_init(&pointer->mutex, NULL);	
 
 }
 
 
 void * incrementa(void* data)
 {
 	for(int i = 0; i<ITERATION; i++)
 	{
 		pthread_mutex_lock(&pointer->mutex);
 		pointer->a++;
 		pointer->b++;
 		pthread_mutex_unlock(&pointer->mutex);
 	}
 	pthread_exit(NULL);
 }
 
 
 int main()
 {
 	pthread_t threads[2];
 	
 	init_struct();
 	
 	for(int i = 0; i<2; i++)
 	{
 		if(pthread_create(&threads[i], NULL, incrementa, NULL) != 0){
 			perror("Error while creating thread");
 			exit(1);
 		}
 	}
 	
 	 
 	for(int i = 0; i<2; i++)
 	{
 		if(pthread_join(threads[i], NULL) != 0){
 			perror("Error while joining thread");
 			exit(1);
 		}
 	}
 	
 	printf("a = %d\n", pointer->a);
 	printf("b = %d\n", pointer->b);
 	
 	exit(0);
 
 
 
 }
 
