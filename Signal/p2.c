#include <signal.h>
#include <stdio.h>
//#include <signal.h>I

int segnale;
static void gestore( int signo );
 
int main() {
	int a = 20, b = 10;

	if ( signal( SIGINT, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );
	else if ( signal( SIGUSR1, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );
	else if ( signal( SIGUSR2, gestore ) == SIG_ERR )	
		printf( "\nNon sono riuscito a catturare il segnale!" );

	do {
		pause();

		if ( segnale == SIGINT )
			printf("\n PROVA! ");
		else if ( segnale == SIGUSR1 )
			printf( "\n tie: %d", a + b );
		else if ( segnale == SIGUSR2 )
			printf( "\n tie: %d", a - b );


	} while( segnale == SIGINT || segnale == SIGUSR1 || segnale == SIGUSR2 );
}

static void gestore( int signo ) {

	signal( SIGUSR1, gestore );
	signal( SIGUSR2, gestore );
	signal( SIGINT, gestore );

	segnale = signo;

}