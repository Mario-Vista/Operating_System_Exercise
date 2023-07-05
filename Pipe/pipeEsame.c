//scrivere un programma che implementi la seguente pipeline
//ls-l|wc -c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int pfd[2], pid;

	if(pipe(pfd) == -1)
	{
		perror("Error while creating pipe");
		exit(-1);
	}

	if(pid = fork())
	{
		close(pfd[1]);
		dup2(pfd[0],0);
		close(pfd[0]);
		execlp("wc", "wc", "-c", NULL);
	}
	else
	{
		close(pfd[0]);
		dup2(pfd[1], 1);
		close(pfd[1]);
		execlp("ls", "ls", "-l", NULL);

	}
}