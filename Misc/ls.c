
//include of my personal path where apue.h library is located
#include "../../../src.3e/apue.3e/include/apue.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(int argc, char *argv[ ])
{
	DIR *dp;

	struct dirent *dirp;

	if (argc!= 2) {
		err_quit("a single argument (the dirname) is required");
	} 

	if ( (dp= opendir(argv[1])) == NULL){
		err_sys("can't open %s", argv[1]);
	}

	while ( (dirp= readdir(dp)) != NULL){

		printf("%s\n", dirp->d_name);
	}

	closedir(dp);

	exit(0);
}