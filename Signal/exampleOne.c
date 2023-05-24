//example of signal use

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src.3e/apue.3e/include/apue.h"

static void handler(int);

int main(){
    int i, n;
    n = fork();

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


        kill(getppid(), SIGUSR1);
        printf("\n(figlio) ho finito e muoio\n");

        exit(0);
    }

    else{
        signal(SIGUSR1, handler);
        printf("\n (padre) il mio process-id e' %d\n", getpid());
        printf("\n incomincio le mie operazioni \n");
        for(i = 0; i<6; i++){
            sleep(1);
            printf("*\n");
        }
        printf("\n(padre) ora muoio anche io\n");
        exit(0);
    }
}

static void handler(int signo){ //operazione alla ricezione di una kill

    if(signo == SIGUSR1)
        printf("\n(padre) RICEVUTO INTERRUPT");
    return;
}