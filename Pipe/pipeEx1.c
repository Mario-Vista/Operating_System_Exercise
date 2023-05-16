/*Esercizio: scrivere un programma che esegua il comando di
shell “ls -R | grep <pat>” dove <pat> è un pattern inserito
dall’utente*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(int argc, char** argv){
    int pfd[2], pid;

    if(pipe(pfd) == -1){
        perror("error creating pipe");
        return -1;
    }

    if(!(pid = fork())){
        close(pfd[0]);
        dup2(pfd[1], 1);
        close(pfd[1]);
        execlp("ls", "ls", "-R", NULL);
    } else{
        close(pfd[1]);
        dup2(pfd[0], 0);
        close(pfd[0]);
        execlp("grep", "grep", argv[1], NULL);
    }

    return 0;
}