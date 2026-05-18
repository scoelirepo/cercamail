#define _GNU_SOURCE // Necessario su alcuni sistemi per getline
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int findpid(char *,char **);
extern int pushpid(int);
extern int poppid(int);


int main(int argc, char *argv[]) {

    FILE *fp,*fp1;
    char *log_line = NULL; //la linea in input
    char session_id[20];
    char *position, *found_session, *found_amavis_session, *session_pid, *found_pid="1"; // per la stringa da cercare
    char amavis_id[20] ={"VUOTO"};
    size_t len = 0; // boh!
    ssize_t read;  // dimensione stringa letta
    int actionpid,status;



    if ( argc != 3 ) {
        printf("Numero argomenti: %d\n", argc - 1);
        printf("USO: cercamail filename session_id\n");
        return 1;
    }

    // nome file da aprre argv[1], id sessione da cercare argv[2]
    printf("Nome file: %s\n", argv[1]);
    printf("Session id:: %s\n", argv[2]);


    // Apre il file argv[1]  in lettura ("r")
    fp = fopen(argv[1], "r");

    // Verifica se il file è stato aperto correttamente
    if (fp == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1; // Termina con codice di errore
    }

    // legge il session_id da cercare
    strcpy(session_id,argv[2]);

    // Legge e stampa il contenuto del file riga per riga
    while ((read = getline(&log_line, &len, fp)) != -1) {


        // Se trovo un'inizio di sessione lo salvo

        // ==>>  Se trovo una stringa che può contenere un pid:
        if ( strstr(log_line,"dovecot") || strstr(log_line,"postfix/smtpd") || strstr(log_line,"postfix/smtps/smtpd") || strstr(log_line,"postfix/submission/smtpd") )
        {
                // restituisce il pid cercato
                status=findpid(log_line,&session_pid);
                if (! status )
                {
                        printf("sessione postfix trovata ma pid non estraibile\n");
                        exit (-1);
                }
                else
                printf ("%s  >>>>>>  pid ==>>>>>  %s\n", log_line, session_pid);

                if ( strstr(log_line," connect from" ) )
                        //  se apertura sessione allora push del pid, se ok restituisce 0
                        printf("eseguo push di %s\n", session_pid);
                        /* actionpid=pushpid(session_pid); */

                if ( strstr(log_line," disconnect from" ) )
                {
                        // pop del pid, se ok restituisce 0, se l'apertura non esiste restituisce 2
                        printf("eseguo pop di %s\n", session_pid);
                        /* actionpid=poppid(session_pid);*/
                }
        }


        // ==>> ora bisogna cercare un pid non chiuso seguito da session_id, quello è il pid voluto
        // altro todo preparare le funzioni pushpid e poppid



        // Ricerca della stringa argv[2] = session-id nel log_line, se trovo analizzo linea
        position = strstr(log_line, session_id);
        if ( position != NULL ) {

                // Cerco stringa relativa alla sessione amavis
                position = strstr(log_line, "postfix/lmtp");

                if ( position != NULL )
                {
                        // cerco l'id dell'ingresso amavis
                        position = strstr(log_line, "queued as ");

                        // leggo in amavis_id la sessione trovata
                        strncpy(amavis_id, position+10, 11);
                        amavis_id[11]='\0'; // chiudo la stringa

                        // non serve altro, chiudo il ciclo ed il file
                        fseek(fp,0,SEEK_END);

                }
        }
    }

    // Chiude il file
    fclose(fp);


   // Output dati

    fp1 = fopen(argv[1], "r");
    if (fp1 == NULL) {
        printf("Errore: Impossibile aprire il file.\n");
        return 1; // Termina con codice di errore
    }

    while ((read = getline(&log_line, &len, fp1)) != -1) {
        // stampo se trovo la sessione mail o quella amavis
        found_session = strstr(log_line, session_id);
        found_amavis_session = strstr(log_line, amavis_id);
        found_pid = strstr(log_line, amavis_id);

        if ( (found_session!=NULL) || (found_amavis_session!=NULL)||(found_pid!=NULL))
                printf("OUTPUT: %s", log_line);
    }


    //free(log_line);
    fclose(fp1);
    free(log_line);

    return 0;

}
