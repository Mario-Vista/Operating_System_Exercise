//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
#include <stdio.h>

int OurSquareRoot(int);

int main(int argc, char *argv[]) {
	int x,s;
	
	printf("Inserisci un intero: ");
	
	scanf("%d",&x);
	
	s=OurSquareRoot(x);
	
	if(s!=-1){
	printf("La radice quadrata di %d: %d\n",x,s);
	}
}