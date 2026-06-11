#define _GNU_SOURCE // Necessario su alcuni sistemi per getline
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

extern int add_pid_to_list(char *,char *);
extern int get_pid(char *,char *);
extern int remove_pid_from_list(char *);
extern int is_pid_in_list(char *);
extern char * get_pid_from_list(char *); // usare strdup 
extern char *pid_stack;




int main(int argc, char *argv[]) {

	FILE *fp,*fp1;
	char *log_line = NULL; //la linea in input
	char 	session_id[20],  // id sessione esad
		session_pid[20], // pid found in first line 
		amavis_id[20], 
		*session_pid_line, 
		*close_session_line,
		*position, 
		*found_session, 
		*found_amavis_session, 
		*found_pid_session, 
		found_pid[20]; 
	size_t len = 0; // boh!
	ssize_t read;  // dimensione stringa letta



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

//	   ===  primo giro ===
//	 leggo linea log_line

	while ((read = getline(&log_line, &len, fp)) != -1) 
	{

//	<> contiene " connect from" ?
//		==> salvo il pid relativo nella lista pids e la stringa in pid_lines 
//	<> contiene la stringa di sessione ?
//		==> se si
//			<> contiene uno dei pid aperti ?
//				==> se si, quello è il pid, lo salvo in session_pid e la linea relativa in session_pid_line 
//			<> contiene anche sessione amavis ?
//				==> se si, lo salvo amavis_id
//	<> contiene " disconnect from" ?
//		<> session_pid è valorizzato ?
//			==> se si
//				<> session_pid è uguale al pid trovato ?
//					==> se si, uscire dal ciclo e fseek(fp,0,SEEK_END)
//			==> se no eliminare il pid dalla lista pids     	


	// ==>>  Se trovo una stringa che può contenere un pid:

		if ( strstr(log_line," connect from") )
		{
			if ( get_pid(log_line,found_pid) == false )
				printf("warning: found connect from but cannot extract pid\n");
			else
				if( add_pid_to_list(found_pid,log_line) == false )
				{
					printf("error: cannot add the pid to the list\n");
					exit(-1);
				}
				else
					printf("DEBUG added pid %s\n",found_pid);
		}

		// ==> Se trovo una stringa che contiene la sessione
		if ( strstr(log_line,session_id)!=NULL )
		{
			if ( get_pid(log_line,found_pid) == false )
				printf("warning: found connect from but cannot extract pid\n");
			else
			{
				if ( is_pid_in_list(found_pid) == true )
				{
					printf("trovato pid e sessione\n");
					strcpy(session_pid,found_pid);
					session_pid_line=strdup(log_line);
				}
			}		
	
	               	position = strstr(log_line, "postfix/lmtp");
	
	                if ( position != NULL )
	                {
	                        // cerco l'id dell'ingresso amavis
	                        position = strstr(log_line, "queued as ");
	
	                        // leggo in amavis_id la sessione trovata
	                        strncpy(amavis_id, position+10, 11);
	                        amavis_id[11]='\0'; // chiudo la stringa
			}
		}
	
	
		//  ==> Se trovo una stringa che contiene la disconnect 
		if ( strstr(log_line," disconnect from")!=NULL )
		{
			if ( get_pid(log_line,found_pid) == false )
				printf("warning: found disconnect from but cannot extract pid\n");
			else
				if(session_pid!=NULL && strcmp(session_pid,found_pid)==0)
				{
					printf("Trovato chiusura pid con foundpid - esco loop 1 \n");
					close_session_line=strdup(log_line);
					fseek(fp,0,SEEK_END);
				}
				else
					if ( remove_pid_from_list(found_pid) == false )
					{
						printf("error: cannot remove the pid to the list\n");
						exit(-1);
					}
					else
						printf("DEBUG removed pid %s\n",found_pid);
		}
	}



			
//   ============================Z // Output dati
//

	fp1 = fopen(argv[1], "r");
	if (fp1 == NULL) 
	{
	    printf("Errore: Impossibile aprire il file.\n");
	    return 1; // Termina con codice di errore
	}

	printf("%s\n",get_pid_from_list(session_pid));
	// PRINT linea dell'elemento della lista con il pid foundpid

	while ((read = getline(&log_line, &len, fp1)) != -1) 
	{
	    // stampo se trovo la sessione mail o quella amavis
	    found_session = strstr(log_line, session_id);
	    found_amavis_session = strstr(log_line, amavis_id);
	    found_pid_session = strstr(log_line, session_pid);

	    if ( (found_session!=NULL) || (found_amavis_session!=NULL)||(found_pid_session!=NULL))
	            printf("OUTPUT: %s", log_line);
	}

	// PRINT  della linea finale copiata sopra 
	printf("%s\n",close_session_line);

	fclose(fp1);
	free(log_line);

	return 0;

}

