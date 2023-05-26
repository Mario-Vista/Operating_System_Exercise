#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//struttura di parametri in input del thread
typedef struct parameter
{
    int start;
    int end;
    int count;
    char *filename;
    char character;

}ThreadParams;


//routine che verrà svolta dal thread
void* routine(void* params)
{
    ThreadParams *p = (ThreadParams*) params;

    //apertura del file in modalità lettura
    FILE *file = fopen(p->filename, "r");

    //restituisco errore se il file non è stato aperto correttamente
    if(file == NULL)
    {
        perror("File non esistente");
        pthread_exit(NULL);
    }

    //metto il cursore all'inizio della parte da esaminare
    fseek(file, p->start, SEEK_SET);

    //conto le occorrenze della lettera voluta
    int ch;
    while((ch = fgetc(file))!= EOF && ftell(file) <= p->end)
    {
        if(ch == p->character)
            p->count++;
        
    }

    //chiusura del file
    fclose(file);

    //chiudo il thread
    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    //scelta numero thread
    int num;
    char filename[100];
    char lettera;


    printf("Quanti thread vuoi usare? ");
    scanf("%d", &num);

    //assegnazione file
    printf("Inserire nome del file di testo ");
    scanf("%s", filename);

    //scelta carattere da ricercare
    printf("Di quale lettera vuoi contare le occorrenze? ");
    scanf(" %c", &lettera);

    //apertura del file in modalità lettura
    FILE *file = fopen(filename, "r");
    
    //restituisco errore se il file non è stato aperto correttamente
    if(file == NULL)
    {
        perror("File non esistente");
        return 1;
    }

    //determino la lunghezza del file
    fseek(file, 0, SEEK_END);
    int file_dim = ftell(file);
    fclose(file);

    //determino la porzione analizzata dai singoli thread
    int portion = file_dim/num;

    //creo e alloco i thread e le rispettive struct dinamicamente
    pthread_t *threads = malloc(num* sizeof(pthread_t));
    ThreadParams *params = malloc(num * sizeof(ThreadParams));

    int sommaTot = 0;

    for(int i = 0; i<num; i++)
    {
        //inizializzo i valori delle struct di parametri
        params[i].character = lettera;
        params[i].filename = filename;
        params[i].start = i*portion;

        //controllo se il thread sia l'ultimo per l'assegnazione della fine della porzione
        params[i].end = (i == num-1) ? file_dim-1 : (i+1) * portion-1;
        params[i].count = 0;

        pthread_create(&threads[i], NULL, routine, (void *) &params[i]);
    }

    for(int i = 0; i<num; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for(int i = 0; i<num; i++)
    {
        sommaTot += params[i].count;
    }

    printf("La lettera %c compare %d volte", lettera, sommaTot);


    // Liberazione della memoria
    free(threads);
    free(params);


    return 0;
}