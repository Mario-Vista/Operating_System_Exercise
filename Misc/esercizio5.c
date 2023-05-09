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
	char buf[BUFFSIZE];

	i = 0;
	somma = 0; 
	memset(buf,0, sizeof(buf));

	while(read(STDIN_FILENO, buf, 2) > 0){
		val = atoi(buf);

		if(val == -1){
			break;
		} else{
			i++;
			somma+=val;

			if(i==2){
				sprintf(buf, "%d", somma);
				buf[strlen(buf)] = '\n';
				write(STDOUT_FILENO, buf, strlen(buf));
				memset(buf, 0, sizeof(buf));
				somma = i = 0;
			}
		}
	}
}