#include "../../../src.3e/apue.3e/include/apue.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

char buf1[] = "abcdefghij";
char buf2[] = "ABCDEFGHIJ";

int main( void ) {
    
    //creo un file descriptor
    int fd;
    if ( ( fd = creat( "file.hole", FILE_MODE ) ) < 0 ) 
        printf("creat error");

    //scrivo il contenuto di buf1 nel file creato
    if ( write( fd, buf1, 10) != 10 ) 
        printf("buf1 write error");

    /* offset ora = 10 */
    
    //mi sposto alla posizione 40
    if ( lseek( fd, 40, SEEK_SET ) == -1 ) 
        printf( "lseek error" );

    /* offset ora = 40 */
    
    //scrivo il contenuto di buf2
    if ( write( fd, buf2, 10 ) != 10 )
        printf( "buf2 write error" );

    /* offset ora = 50 */
    exit(0);
}