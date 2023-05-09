//programma che permette di identificare univocamente l'utente e il gruppo di appartenenza

//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdio.h>

int main(void) {
	
	printf("User id= %d, Group id= %d\n", getuid(), getgid());
	exit(0);
}