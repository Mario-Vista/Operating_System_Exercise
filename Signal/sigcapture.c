//example of code that catch various signal and print the type

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../../src.3e/apue.3e/include/apue.h"

static void sig_usr(int);

int main(void){

    if(signal(SIGUSR1, sig_usr) == SIG_ERR)
        printf("can't catch SIGUSR1");

    if(signal(SIGUSR2, sig_usr) == SIG_ERR)
        printf("can't catch SIGUSR2");

    if(signal(SIGINT, sig_usr) == SIG_ERR)
        printf("can't catch SIGINT");

    if(signal(SIGTSTP, sig_usr) == SIG_ERR)
        printf("can't catch SIGTSTP");

    for( ; ; )
    pause();


}


static void sig_usr(int signo)// signo è il numero del segnale
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else if (signo == SIGINT)
        printf("received SIGINT\n");
    else if (signo == SIGTSTP)
        printf("received SIGTSTP\n");
    else
        printf("received signal %d\n", signo);
    
    return;
}
