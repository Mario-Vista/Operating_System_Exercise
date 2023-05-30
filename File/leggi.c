// Scrivere un programma C “leggi.c” che
// utilizzando la funzione primitiva “read”,
// legga il contenuto del file “alfabeto.txt” e
// lo stampi sullo standard output.

#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
    int fd, nread;
    char buff[1000];

    //apro il file in modalità lettura
    fd = open("alfabeto.txt", O_RDONLY);

    //gestione errori
    if(fd == -1){
        perror("Error during file opening\n");
        exit(1);
    }

    //finchè la condizione è vera ci sono ancora dati da leggere all'interno del file e li memorizza in buff
    while((nread = read(fd, buff, 1000)) > 0){
        //scrive i dati letti in buff nel file descriptor associato allo standard di output
        if(write(STDOUT_FILENO, buff, nread) == -1){
            perror("Error while writing");
            close(fd);
            exit(1);
        }
    }
    write(STDOUT_FILENO, "\n", 1);

    close(fd);
    exit(0);
}