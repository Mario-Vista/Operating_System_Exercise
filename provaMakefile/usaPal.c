/* usaPal.c */
#include <stdio.h>
#include "palindroma.h"

int main (int argc, char *argv[]) {
	
	if (palindroma(argv[1]))
	
		printf ("La stringa \"%s\" e` palindroma.\n", argv[1]);
	
	else
		printf ("La stringa \"%s\" non e` palindroma.\n", argv[1]);
}