//scrivi.c

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

int main(void){
	int fd,nwrite;
	char *data = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	fd = open("alfabeto.txt", O_WRONLY | O_CREAT, S_IRUSR| S_IWUSR);

	if(fd == -1){
		perror("errore apertura");
		exit(-1);
	}

	nwrite = write(fd,data,strlen(data));

	if(nwrite == -1){
		perror("errore scrittura");
		exit(-1);
	}

	close(fd);
	exit(0);
}
