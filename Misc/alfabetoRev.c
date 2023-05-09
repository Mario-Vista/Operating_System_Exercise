#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
  
#define BUFDIM   1000

int main(int argc, char **argv){

int fd, nread;
int i =1;
int currpos;
char buffer[BUFDIM];

if(argc != 2){
  perror("errore\n");
  exit(1);
}

  if((fd = open(argv[1], O_RDONLY)) < 0){
    perror("apertura sorgente: ");
    exit(1);
  }

  if(lseek(fd, 0, SEEK_END)== -1){
    perror("lseek errore");
    close(fd);
    exit(1);
  }

  currpos = lseek(fd,0,SEEK_CUR);
  currpos = currpos-i;

  if(lseek(fd, currpos, SEEK_SET) == -1){
    perror("lseek errore");
    close(fd);
    exit(1);
  }


  while((nread = read(fd, buffer, 1)) > 0){
      if(write(STDOUT_FILENO, buffer, nread) == -1){
      perror("errore in scrittura");
      close(fd);
      exit(1);
  }

  currpos--;

  if(currpos >=0){
      if(lseek(fd, currpos, SEEK_SET) == -1){
      perror("lseek errore");
      close(fd);
      exit(1);
      } 
  } else {

    write(STDOUT_FILENO, "\n", 1);
    close(fd);
    exit(0);
    }
 }

}