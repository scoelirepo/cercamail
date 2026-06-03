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
 <> leggo linea log_line
         ==> contiene la stringa di sessione ? 
            ==> se si, contiene anche sessione amavis ?
                ==> se si, salvo amavis_id
            la stringa di sessione contenuta in log_line contiene anche un pid e soddisfa le condizioni di inizio sessione ( vedi sotto )?
                ==> se si, verifico se il pid è in lista, se c'è allora è quello il pid da usare per cercare la chiusura connessione, se no errore

      contiene una stringa di inizio connessione ?  " connect from"
           ==> se si, estraggo pid e lo salvo nella lista
      contiene stringa di fine connessione ? " disconnect from"
           ==> se si, estraggo pid e lo elimino dalla lista se vi esiste

 
-- secondo giro
 
  condizioni di inizio sessione : if ( strstr(log_line,"dovecot") || 
  strstr(log_line,"postfix/smtpd") || 
  strstr(log_line,"postfix/smtps/smtpd") || 
  strstr(log_line,"postfix/submission/smtpd") )



