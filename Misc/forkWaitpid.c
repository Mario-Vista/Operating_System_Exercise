#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"

int main(void){
	char buf[MAXLINE];
	pid_t pid;
	int status;

	printf("%% \n");

	while(fgets(buf, MAXLINE, stdin) != NULL){
		buf[strlen(buf)-1] = 0;

		if((pid == fork) < 0){
			err_sys("fork error");
		} else if(pid == 0){
			execlp(buf,buf,(char *) 0);
			err_ret("couldn't execure \n");

		} if((pid = waitpid(pid, &status, 0)) < 0){
			err_sys("waitpid error");
			printf("&&");
		}	
	} exit(0);
}
