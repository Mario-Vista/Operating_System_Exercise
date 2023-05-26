#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struttura dati per i parametri del thread
typedef struct {
    char* filename;
    char character;
    int start;
    int end;
    int count;
} ThreadParams;

// Funzione eseguita dal thread
void* count_occurrences(void* arg) {
    ThreadParams* params = (ThreadParams*)arg;

    // Apertura del file in modalità lettura
    FILE* file = fopen(params->filename, "r");
    if (file == NULL) {
        perror("Errore durante l'apertura del file");
        pthread_exit(NULL);
    }

    // Impostazione del cursore del file all'inizio della porzione da esaminare
    fseek(file, params->start, SEEK_SET);

    // Conteggio delle occorrenze del carattere nella porzione di file
    int ch;
    while ((ch = fgetc(file)) != EOF && ftell(file) <= params->end) {
        if (ch == params->character)
            params->count++;
    }

    // Chiusura del file
    fclose(file);

    pthread_exit(NULL);
}

int main() {
    int n;  // Numero di thread
    char filename[100];  // Nome del file di testo
    char character;  // Carattere da cercare

    printf("Inserisci il numero di thread: ");
    scanf("%d", &n);

    printf("Inserisci il nome del file di testo: ");
    scanf("%s", filename);

    printf("Inserisci il carattere da cercare: ");
    scanf(" %c", &character);

    // Apertura del file per determinare la sua lunghezza
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Errore durante l'apertura del file");
        return 1;
    }

    // Determinazione della lunghezza del file
    fseek(file, 0, SEEK_END);
    int file_length = ftell(file);
    fclose(file);

    // Calcolo della dimensione delle porzioni di file per ogni thread
    int portion_size = file_length / n;

    // Creazione dei thread
    pthread_t* threads = malloc(n * sizeof(pthread_t));
    ThreadParams* params = malloc(n * sizeof(ThreadParams));

    int i;
    for (i = 0; i < n; i++) {
        params[i].filename = filename;
        params[i].character = character;
        params[i].start = i * portion_size;
        params[i].end = (i == n - 1) ? file_length - 1 : (i + 1) * portion_size - 1;
        params[i].count = 0;

        pthread_create(&threads[i], NULL, count_occurrences, (void*)&params[i]);
    }

    // Attende la terminazione dei thread
    for (i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    // Somma dei conteggi di occorrenze da tutti i thread
    int total_count = 0;
    for (i = 0; i < n; i++) {
        total_count += params[i].count;
    }

    printf("Il carattere '%c' compare %d volte nel file.\n", character, total_count);

    // Liberazione della memoria
    free(threads);
    free(params);

    return 0;
}
