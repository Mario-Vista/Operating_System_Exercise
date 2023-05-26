/* Scrivere un programma che prende in input un intero n, il nome di
un file di testo ed un carattere x
• Il programma ha il compito di contare le occorrenze del carattere x nel file di testo
• Il programma esegue tale compito creando n thread, ognuno dei
quali esamina una porzione diversa del file di testo
• ad esempio, se il file è lungo 1000 bytes ed n=4, il primo threadesaminerà i primi 250 bytes, il secondo thread esaminerà i 250 bytes successivi, e così via */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

char lettera;

typedef struct params{
    char *file;
    int  pfile;
    int byte;
} paramet;

void *routine(void *parametri){
    paramet* p = (paramet *) parametri;

    

}


int main(int argc, char* argv[]){
    pthread_t th[4];
    char *file;
    paramet miastruct;
    int count;

    printf("che file vuoi?\n");
    scanf("%s", &file);

    printf("che caratteree vuoi?\n");
    scanf("%s", &lettera);

    FILE *fptr;
    fptr = fopen(file,"r");
    int n = lseek(fptr, 0, SEEK_END);
    printf("Dimensione del file: %d\n", n);
    printf("Numero di byte per thread: %d\n", n/4);
    void* threadReturn;
    int localCount;
    int curr = 0;




    for(int i = 0; i<4; i++){
        miastruct.byte = n;
        miastruct.file = file;
        miastruct.pfile = curr+n;
        if(pthread_create(&th[i], NULL, &routine, (void*) &miastruct)!= 0){
            return i;
        }
        pthread_join(th[i], (void*) &localCount);
        count+=localCount;
    }
    printf("Count totale: %d\n", count);
}   