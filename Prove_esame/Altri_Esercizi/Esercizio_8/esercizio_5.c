/**
 * ESERCIZIO: 5
 * 
 * Si realizzi un programma C e Posix in ambiente Linux che, impiegando la libreria Pthread, 
 * crei un vettore di interi di dimensione n, dove n >= 24 e multiplo di 4,
 * è un argomento passato da riga di comando.
 *   
 * Calcolati i valori medi degli elementi contenuti nei quattro sottovettori di taglia n/4,  
 * il programma deve determinare, impiegando quattro thread,
 * la somma del numero di elementi minori del valore medio in ciascun sottovettore.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#define FOUR 4

//numero inserito in input
int N;

//dimensione dei sottoarray
int portion;

//array
int *array;

//function per il calcolo della media
int average(int start, int end, int number)
{
    int parsum = 0;
    for(int i = start; i<end; i++)
    {
        parsum += array[i];
    }
    return parsum/number;
}


//routine eseguita dal thread
void *routine(void *data)
{
    //indice di inizio del sottovettore
    int *st = (int *) data;

    //indice di fine del sottovettore
    int end = *st+portion-1;

    //media
    int avg = average(*st, end, portion);
    
    //somma degli elementi minori della media
    int parsum;
   

    for(int i = *st; i<end; i++)
    {
        if(array[i] < avg)
        {
            parsum+=array[i];
        }
    }

    //stampe
    printf("\nStart: %d, End: %d\n", *st, end);
    printf("Media: %d\n", avg);
    for(int i = *st; i<end; i++)
    {
        printf("%d ",array[i]);
    }

    printf("\nSomma porzione: %d\n", parsum);

    pthread_exit(NULL);

}



int main(int argc, char const *argv[])
{

    //prendo la dimensione da riga di comando
    N = atoi(argv[1]);

    //controllo che la dimensione sia giusta
    if(N < 24 || (N%4) != 0)
    {
        perror("Assicurati che il numero sia >= 24 e divisibile per 4");
        exit(1);
    }

    //allocazione dinamica dell'array
    array = malloc(N * sizeof(int));

    //assegnazione di valori randomici compresi fra 1 e 10
    for(int i = 0; i<N; i++)
    {
        array[i] = rand()%11;
    }

    //size dei sottovettori
    portion = N/FOUR;

    //dichiarazione dell'array di indici di partenza
    int start[portion];

    //creo i 4 thread
    pthread_t threads[FOUR];

    for(int i = 0; i<FOUR; i++)
    {
        //calcolo il valore degli indici di inizio dei sottovettori
        start[i] = i*portion;


        if(pthread_create(&threads[i], NULL, routine, (void *) &start[i])!= 0)
        {
            perror("Error while creating thread");
            exit(-1);
        }
    }

    
    for(int i = 0; i<FOUR; i++)
    {
        if(pthread_join(threads[i], NULL)!= 0)
        {
            perror("Error while joining thread");
            exit(-1);
        }
    }




    exit(1);
}
