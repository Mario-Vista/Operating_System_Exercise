#include <signal.h>
#include <stdio.h>
//#include <signal.h>

int segnale;
static void gestore( int signo );
 
int main() {
	if ( signal( SIGINT, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale !" );

	do {
		pause();

		if ( segnale == SIGINT )
			printf("\n Don't stop me now! ");


	} while( segnale == SIGINT );
}

static void gestore( int signo ) {
	signal( SIGINT, gestore );
	segnale = signo;
}