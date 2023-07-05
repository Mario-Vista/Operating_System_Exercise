#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * ESEMPIO: Stampa informazioni sui file
*/

int main(int argc, char const *argv[])
{
	//dichiaro una variabile di tipo struct stat per memorizzare le impostazioni sul file
	struct stat buf;

	//puntatore a stringa
	char *ptr;

	for(int i = 0; i < argc; i++)
	{
		printf("%s: ",argv[i]);

		//la chiamata lstat restituisce un oggetto di tipo struct stat 
		if(lstat(argv[i], &buf) < 0)
		{
			printf("lstat error\n");
			continue;
		}
		//il campo st_mode della struct stat contiene informazioni sui permessi del file
		if (S_ISREG(buf.st_mode))
			ptr = "regular";
		else if( S_ISBLK(buf.st_mode) )
			ptr = "block special";
		else
			ptr = "** unknown mode **";
		printf("%s\n", ptr);
	}	
	exit(0);
}