/* palindroma.c */
#include <string.h>
#include "reverse.h"
#include "palindroma.h"


int palindroma (char *str) {

	/* Buffer per la stringa invertita */
	char revstr [strlen(str)+1];

	/* Inverte la stringa in ingresso */

	reverse (str, revstr);
	/* vero se le stringhe sono uguali */

		return !strcmp(str, revstr);
}