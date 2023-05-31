// Si realizzi un programma in C e Posix sotto Linux che,
// utilizzando la libreria Pthread
// • lancia n thread per cercare un elemento in una matrice nxn di
//  caratteri
// • Ognuno dei thread cerca l'elemento in una delle righe della
//  matrice
// • Non appena un thread ha trovato l'elemento cercato, rende note
//  agli altri thread le coordinate dell'elemento e tutti i thread
//  terminano (sono cancellati)

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

    
//matrice globale
int matrix[5][5];

//flag per controllare se la parola è già stata trovata
int found = 0;

//inizializzazione statica del mutex
pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;

//struct che contiene i dati che vengono passati in input ai thread
typedef struct data_struct
{
    int row;
    int find;
}data_struct;

//routine eseguita dal thread, effettua la ricerca del numero dato su una riga della matrice
void *search(void* data)
{
    //alloca un puntatore struct facendo il cast
    data_struct *d = (data_struct *) data;

    //scorre la riga della matrice assegnata all'i-esimo thread
    for(int i = 0; i< 5; i++)

        //controllo se il numero è stato trovato
        if(matrix[d->row][i] == d->find)
        {
        
        //entra in sezione critica
        pthread_mutex_lock(&my_mutex);

        //controlla se il numero richiesto è già stato trovato
        if(found == 0)
        {
            //se non è stato ancora trovato imposta la flag a uno
            found = 1;
            printf("Trovata la lettera nella cella %d, %d\n", d->row, i);
            pthread_mutex_unlock(&my_mutex);


        }
        else
        {
            //in caso sia stato già trovato il processo termina
            pthread_mutex_unlock(&my_mutex);
            printf("Sto terminando \n");
            pthread_exit(NULL);

        }
        }

        //termino il thread
        pthread_exit(NULL);


}



int main()
{

    //variabili
    int find_num;
    pthread_t th[5];
    data_struct *data = malloc(5*(sizeof(data_struct)));

    srand(time(NULL));


    //creo la matrice e assegno i valori
    for(int i = 0; i<5; i++){
        for(int j = 0; j<5; j++){
            matrix[i][j] = rand()%20;
        }
    }


    //stampa della matrice
    int row, columns;
    for (row=0; row<5; row++)
    {
        for(columns=0; columns<5; columns++)
        {
            printf("%d     ", matrix[row][columns]);
        }
        printf("\n");
    }

    //chiedo elemento da ricercare
    printf("Quale numero vuoi ricercare? ");
    scanf("%d", &find_num);




    for(int i = 0; i<5; i++)
    {
        //assegno alla struct il numero della riga su cui il thread deve lavorare e il numero da ricercare
        data[i].row = i;
        data[i].find = find_num;

        //creo i thread
        if(pthread_create(&th[i], NULL, search, (void *) &data[i]) != 0){
            perror("Error while creating thread");
            exit(1);
        }


    }

    //effettuo la join per tutti i thread
    for(int i = 0; i<5; i++){
        if(pthread_join(th[i], NULL) != 0){
            perror("Error while joining thread");
            exit(1);
        }
    }




}