#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int findpid(char *logline, char **ilpid )
{
        char *stringa_da_verificare;
        char *token;
        const char delimitatori[] = "[]";

        // preparo la stringa su cui cercare modificabile
        if((stringa_da_verificare=strdup(logline))==NULL) {
                printf("strdup fallito\n");
                return 0;
        }


        //  split usando "[" e "]"
        if((token = strtok(stringa_da_verificare, delimitatori))==NULL) {
                printf("nom trovo primo token\n");
                free(stringa_da_verificare);
                return 0;
        }
        printf ("debug: primo token %s\n",token);


        if((token = strtok(NULL, delimitatori))==NULL) {
                printf("nom trovo secondo token\n");
                free(stringa_da_verificare);
                return 0;
        }
        printf ("debug: questo est secondo token che deve essere il pid %s\n",token);

        // copio il token nella stringa preparata nel main usando il suo puntatore
        strcpy(token,*ilpid);

        free(stringa_da_verificare);
        return 1;
}


int pushpid(int pid)
{
        return 0 ;
}


int poppid(int pid)
{
        return 0 ;
}
