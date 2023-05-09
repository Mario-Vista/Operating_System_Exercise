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

#define BUFFSIZE 100

int main(void){
	int somma, i, val;
	int fd1, fd2;
	char buf[BUFFSIZE];

	fd1= open("testfile.txt", O_RDONLY);

	if(fd1 == -1){
		perror("errore apertura");
		exit(-1);
	}

	fd2= open("outfile.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if(fd2 == -1){
		perror("errore apertura");
		exit(-1);
	}	

	i = 0;


	}
