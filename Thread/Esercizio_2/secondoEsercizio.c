/*Scrivere un programma che accetta un intero n da riga di comando,
 crea n thread e poi aspetta la loro terminazione
Ciascun thread aspetta un numero di secondi casualetra 1 e 10, 
poi incrementa una variabile globale interaed infine ne stampa il valore*/


#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int count = 0;

void *routine(){
    int num = rand()%10+1;
    sleep(num);
    count++;
    printf("Valore della variabile: %d\n", count);
}

int main(int argc, char* argv[]){
    srand(time(NULL));
    int threadnum = atoi(argv[1]);

    pthread_t th[threadnum];

    for(int i = 0; i<threadnum; i++){
        
        
        if(pthread_create(&th[i], NULL, &routine, NULL)!= 0){
            return i;
        }

        pthread_join(th[i], NULL);
    }

}
