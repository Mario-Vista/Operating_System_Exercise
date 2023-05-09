#include <stdio.h>
//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"

int main(void){
	printf("hello world from process ID \n", getpid());
	exit(0);
}