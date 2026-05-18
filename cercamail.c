#define _GNU_SOURCE // Necessario su alcuni sistemi per getline
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {

    FILE *fp;
    char *log_line = NULL; //la linea in input
    char pattern[20];
    char *posizione, *header1, *header2; // per la stringa da cercare
    char amavisid[20] ={"VUOTO"};
    size_t len = 0; // boh!
    ssize_t read;  // dimensione stringa letta



    if ( argc != 3 ) {
        printf("Numero argomenti: %d\n", argc - 1);
        printf("USO: cercamail filename sessionid\n");
        return 1;
    }

    printf("Nome file: %s\n", argv[1]);
    printf("Session id:: %s\n", argv[2]);


    // Apre il file argv[1]  in lettura ("r")
    fp = fopen(argv[1], "r");

    // Verifica se il file è stato aperto correttamente
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1; // Termina con codice di errore
    }

    // legge il pattern da cercare
    strcpy(pattern,argv[2]);

    // Legge e stampa il contenuto del file riga per riga
    while ((read = getline(&log_line, &len, fp)) != -1) {

         // Ricerca della stringa argv[2] = session-id nel log_line, se trovo analizzo linea
        posizione = strstr(log_line, pattern);
        if ( posizione != NULL ) {

                // Cerco stringa relativa alla sessione amavis
                posizione = strstr(log_line, "postfix/lmtp");

                if ( posizione != NULL )
                {
                        // cerco l'id dell'ingresso amavis
                        posizione = strstr(log_line, "queued as ");

                        // leggo in amavisid la sessione trovata
                        strncpy(amavisid, posizione+10, 11);
                        amavisid[11]='\0'; // chiudo la stringa

                        // non serve altro, chiudo il ciclo ed il file
                        fseek(fp,0,SEEK_END);

                }
        }
    }

    // Chiude il file
    free(log_line);
    fclose(fp);


   // Output dati

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1; // Termina con codice di errore
    }

    while ((read = getline(&log_line, &len, fp)) != -1) {
        // stampo se trovo la sessione mail o quella amavis
        header1 = strstr(log_line, pattern);
        header2 = strstr(log_line, amavisid);
        if ( (header1!=NULL) || (header2!=NULL) )
                printf("%s", log_line);
    }


    free(log_line);
    fclose(fp);

    return 0;

}
