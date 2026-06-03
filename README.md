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
            <> contiene anche sessione amavis ?
                ==> se si, salvo amavis_id
            <> contiene uno dei pid in lista ?
                ==> se si, salvo il pid in session_pid e la linea in session_pid_line
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



