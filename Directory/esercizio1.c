    /**
     * PARZIALE: Prova Scritta 14/07/2022
    */

    /**
     * 6 punti
     *
     * Scrivere un programma in C che, in ambiente Linux, presa una directory come parametro da riga di comando,
     * provvederà a scorrerla allo scopo di stampare i nomi di tutti i file regolari e le loro dimensioni contenuti in tale
     * directory se questi hanno i diritti di scrittura e lettura per il proprietario e per gli altri.
    */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        perror("Error in input parameters");
        exit(1);
    }

    // puntatore alla directory aperta
    DIR *dp;

    // strutture cartella e file contenuti
    struct dirent *dir;
    struct stat fs;

    // apro la directory inserita da riga di comando
    dp = opendir(argv[1]);

    if (dp == NULL) {
        perror("opendir");
        exit(1);
    }

    //ciclo while che legge le voci della directory
    while ((dir = readdir(dp)) != NULL) {
        
        //nome del percorso
        char* fileName = dir->d_name;
        
        // Informazioni utili
        printf( "\nVerificando: <path-directory>/%s -> ", fileName );

        // Recupero informazioni del file utilizzando stat 
        stat( fileName, &fs );

        if ( S_ISREG( fs.st_mode ) && // file regolare
            ( fs.st_mode & S_IRUSR ) && ( fs.st_mode & S_IWUSR ) &&  // read & write owner
            ( fs.st_mode & S_IRGRP ) && ( fs.st_mode & S_IWGRP ) &&  // read & write group
            ( fs.st_mode & S_IROTH ) && ( fs.st_mode & S_IWOTH ) ) { // read & write others

            // Se supera i precendenti controlli   
            printf( "bytes: %ld", fs.st_size );

        } else {

            // Se non supera i precendenti controlli 
            printf( "non rispetta le verifiche." );
        }
    }

    // Chiusura directory
    closedir( dir );
    printf( "\n" );
    exit( EXIT_SUCCESS );
}