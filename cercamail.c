#define _GNU_SOURCE // Necessario su alcuni sistemi per getline
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int findpid(char *,char *);
extern int addpid(char *);
extern int delpid(char *);
extern char *pid_stack;

int main(int argc, char *argv[]) {

	FILE *fp,*fp1;
	char *log_line = NULL; //la linea in input
	char session_id[20], session_pid[10], amavis_id[20], *session_pid_line;

	char *position, *found_session, *found_amavis_session, *found_pid="1"; // per la stringa da cercare
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
	// Copiando le righe del file fp in log_line; len contiene la lunghezza
	while ((read = getline(&log_line, &len, fp)) != -1) {

		/*	   ===  primo giro ===
			[] leggo linea log_line
			<> contiene " connect from" ?
				==> salvo il pid relativo nella lista pids e la stringa in pid_lines 
			<> contiene la stringa di sessione ?
				==> se si
				<> contiene uno dei pid aperti ?
					==> se si, quello è il pid, lo salvo in session_pid e la linea relativa in pid_lines 
				<> contiene anche sessione amavis ?
					==> se si, lo salvo amavis_id
			<> contiene " disconnect from" ?
				<> session_pid è valorizzato ?
					<> session_pid è uguale al pid trovato ?
					==> se si, uscire dal ciclo
				==> eliminare il pid dalla lista pids     	
		*/

	// ==>>  Se trovo una stringa che può contenere un pid:

	if ( strstr(log_line," connect from")!=NULL )
	{
		if( get_pid(&log_line,&found_pid)==NULL)
		{
			printf("warning: found connect from but cannot extract pid\n");
		}
		else
			if( add_pid_to_list(found_pid)==NULL )
			{
				printf("error: cannot add the pid to the list\n");
				exit(-1);
			}
			else printf("DEBUG added pid %s\n",found_pid);
	}


	// ==> Se trovo una stringa che contiene la sessione
	if ( (position=strstr(log_line,session_id))!=NULL )
	{

              findpid=searchpidstatus=findpid(log_line,found_pid);
//                if (! status )
//                {
//                        printf("sessione postfix trovata ma pid non estraibile\n");
//                        exit (-1);
//                }
//







//    if ( strstr(log_line,"dovecot") || strstr(log_line,"postfix/smtpd") || strstr(log_line,"postfix/smtps/smtpd") || strstr(log_line,"postfix/submission/smtpd") )
//        {
//                // restituisce il pid cercato
//                status=findpid(log_line,found_pid);
//                if (! status )
//                {
//                        printf("sessione postfix trovata ma pid non estraibile\n");
//                        exit (-1);
//                }
//                else
//                printf ("%s  >>>>>>  pid > %s inserito in session_pid\n", log_line, found_pid);
//
//                if ( strstr(log_line," connect from" )!=NULL )
//		{
//                        //  se apertura sessione allora push del pid, se ok restituisce 0
//                        printf("eseguo push di %s\n", found_pid);
//                        actionpid=addpid(session_pid);
//		}
//
//                if ( strstr(log_line," disconnect from" )!=NULL )
//                {
//                        // pop del pid, se ok restituisce 0, se l'apertura non esiste restituisce 2
//                        printf("eseguo pop di %s\n", found_pid);
//                        actionpid=delpid(found_pid);
//                }
//        }
//
//
//        // ==>> ora bisogna cercare un pid non chiuso seguito da session_id, quello è il pid voluto
//        // altro todo preparare le funzioni addpid e delpid
//
//
//
//        // Ricerca della stringa argv[2] = session-id nel log_line, se trovo analizzo linea
//        position = strstr(log_line, session_id);
//        if ( position != NULL ) 
//	{
//		// Verifico se è uno dei pid appena aperti 
//		
//		findpid=searchpidstatus=findpid(log_line,found_pid);
//                if (! status )
//                {
//                        printf("sessione postfix trovata ma pid non estraibile\n");
//                        exit (-1);
//                }
//
//
//                // Cerco stringa relativa alla sessione amavis
//                position = strstr(log_line, "postfix/lmtp");
//
//                if ( position != NULL )
//                {
//                        // cerco l'id dell'ingresso amavis
//                        position = strstr(log_line, "queued as ");
//
//                        // leggo in amavis_id la sessione trovata
//                        strncpy(amavis_id, position+10, 11);
//                        amavis_id[11]='\0'; // chiudo la stringa
//
//                        // non serve altro, chiudo il ciclo ed il file
//                        fseek(fp,0,SEEK_END);
//
//                }
//        }
//    }
//
// Chiude il file
	fclose(fp);
//
//
//   // Output dati
//
	fp1 = fopen(argv[1], "r");
	if (fp1 == NULL) {
	    printf("Errore: Impossibile aprire il file.\n");
	    return 1; // Termina con codice di errore
	}

	while ((read = getline(&log_line, &len, fp1)) != -1) {
	    // stampo se trovo la sessione mail o quella amavis
	    found_session = strstr(log_line, session_id);
	    found_amavis_session = strstr(log_line, amavis_id);
	    found_pid = strstr(log_line, session_pid);

	    if ( (found_session!=NULL) || (found_amavis_session!=NULL)||(found_pid!=NULL))
	            printf("OUTPUT: %s", log_line);
	}


	//free(log_line);
	fclose(fp1);
	free(log_line);

	return 0;

}



/*

# cercamail
In a mail log or in system journal search for email sessions information

Notes on build:

// legge il session_id da cercare
	strcpy(session_id,argv[2]);

 ** Le variabili principali sono le seguenti **
 log_line   contiene la linea letta dal file, viene usata per cercare le stringhe
 session_id     id sessione mail
 session_pid    pid della sessione mail 
 amavis_id      id sessione amavis
	
	
	
Il file viene letto una riga alla volta una prima volta, 

In ogni riga letta cerchiamo un inizio connessione ed estraiamo il pid, se ne troviamo uno lo aggiungiamo alla lista,
cerchiamo anche un fine connessione e se lo troviamo rimuoviamo il pid dalla lista, in questo modo abbiamo sempre
la lista dei pid attivi

In ogni riga viene anche cercata la sessione che si trova nella stringa session_id, 
ogni volta che viene trovata una riga con la sessione, verifichiamo se contiene anche una sessione amavis e se si 
la sessione amavis viene salvata in amavis_id. In questo modo la seconda passata visualizzerà anche la sessione amavis

Se si trova una linea con la sessione e un pid, si verifica se il pid è in lista. Se non c'è siamo in errore, se c'è 
quello è il pid che dovrà essere usato per visualizzare la linea di chiusura connessione, che deve essere l'ultima



--   primo giro
[] leggo linea log_line
	<> contiene " connect from" ? 
	    ==> salvo il pid relativo nella lista 
	<> contiene la stringa di sessione ? 
	    ==> se si 
	<> contiene uno dei pid aperti ?
	   ==> se si, quello è il pid, salvalo in pid sessione e la stringa in session_pid_line
	    <> contiene anche sessione amavis ?
	        ==> se si, salvo amavis_id
	<> session_pid è valorizzato e ho trovato una linea che lo contiene e contiene " disconnect from" ?
	    ==> se si, la ricerca nel file è finita



 
-- secondo giro
==> output di session_pid_line
[] leggo linea log_line
	<> contiene la stringa di sessione o pid ? 
	    ==> output linea
	    ==> session_started = true
	<> contiene disconnect from e il pid ?
	    <> session started == true ?
	        ==> si, output linea e chiusura
	    
	    
 
  *condizioni di inizio sessione*
  if ( strstr(log_line,"dovecot") || 
  strstr(log_line,"postfix/smtpd") || 
  strstr(log_line,"postfix/smtps/smtpd") || 
  strstr(log_line,"postfix/submission/smtpd") )


*/
