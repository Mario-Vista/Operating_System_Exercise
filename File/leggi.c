//leggi.c

//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
	
#define BUFDIM	 1000

int main(void){

	int fd;
	char buffer[BUFDIM];

	fd = open("alfabeto.txt", O_RDONLY);


	if (fd == -1) { 
    printf("Errore: impossibile aprire il file out.bin\n");
    
    exit(1);
  	}

  	int nLett = 0;
  while((nLett = read(fd, buffer, BUFDIM)) > 0) {
    if(write(STDOUT_FILENO, buffer, nLett) == -1){
    	perror("errore in scrittura");
    	close(fd);
    	exit(1);
    }
  }

  write(STDOUT_FILENO, "\n", 1);
 
  close(fd);
  exit(0);

}