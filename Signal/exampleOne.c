//example of signal use

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src.3e/apue.3e/include/apue.h"

static void handler(int);

int main(){
    int i, n;

    //effettuo la fork del processo
    n = fork();

    //gestisco gli errori
    if(n==-1){
        fprintf(stderr, "fork fallita\n");
        exit(1);
    }
     else if( n==0 ){    //processo figlio
        printf("\n (figlio) il mio process-id e' %d\n", getpid());
        printf("\n (figlio) aspetto 3 secondi e invio un seganale a %d\n", getppid());
        for(i = 0; i<3; i++){
            sleep(1);
            printf(".\n");
        }

        //invia un segnale personalizzato al padre del processo
        kill(getppid(), SIGUSR1);
        printf("\n(figlio) ho finito e muoio\n");

        //il processo figlio termina
        exit(0);
    }

    //blocco eseguito dal padre
    else{
        //registra l'handler per il segnale SIGUSR1
        signal(SIGUSR1, handler);
        printf("\n (padre) il mio process-id e' %d\n", getpid());
        printf("\n incomincio le mie operazioni \n");
        //aspetta 6 secondi
        for(i = 0; i<6; i++){
            sleep(1);
            printf("*\n");
        }
        //il padre termina
        printf("\n(padre) ora muoio anche io\n");
        exit(0);
    }
}

//funzione che gestisce il gestore
static void handler(int signo){ //operazione alla ricezione di una kill

    //se il segnale è SIGUSR1
    if(signo == SIGUSR1)
    //il padre ha ricevuto l'interrupt
        printf("\n(padre) RICEVUTO INTERRUPT");
    return;
}