#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int main(int argc, char* argv[]){
    int fd, nwrite;
    
    //il file apre il file in modalità scrittura, se non esiste lo crea e assegna i permessi in lettura e scrittura all'utente
    fd = open("alfabeto.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    if(fd == -1){
        perror("Error during file opening\n");
        exit(1);
    }

    nwrite = write(fd, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));

    if(nwrite == -1){
        perror("Error during writing file\n");
        exit(-1);
    }

    close(fd);
    exit(0);
}