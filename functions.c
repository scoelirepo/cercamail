#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int add_pid_to_list(char *,char *)
{ return 0;
};

int get_pid(char **,char **)
{ return 0;
};

int remove_pid_from_list(char *)
{ return 0;
};

int is_pid_in_list(char *)
{ return 0;
};

char * get_pid_from_list(char *) // usare strdup
{
	return NULL;
}

char *pid_stack;



/*

mystack *stackpointer=NULL,*stackroot=NULL;

int add_pid(char *ilpid)
{
	if (! stackpointer )
	{	
		stackpointer=malloc(sizeof(mystack));	
		stackroot=stackpointer;
		stackpointer->sessionpid=strdup(ilpid);
		stackpointer->nextelement=NULL;
		stackpointer->prevelement=NULL;
	}
	else
	{
		stackpointer->nextelement=malloc(sizeof(mystack));

		// CHE COSA NON VA ?  *(mystack)(stackpointer->nextelement).sessionpid=strdup(ilpid);

		stackpointer->prevelement=stackpointer;
		stackpointer=stackpointer->nextelement;
	}
}

int delpid(char *ilpid)
{
	return 0;
}


int findpid(char *logline, char *ilpid )
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
        printf ("debug: primo token: \"%s\"\n",token);


        if((token = strtok(NULL, delimitatori))==NULL) {
                printf("nom trovo secondo token\n");
                free(stringa_da_verificare);
                return 0;
        }


        // copio il token nella stringa ilpid
        strncpy(ilpid, token, sizeof(ilpid) - 1);
        ilpid[sizeof(ilpid) - 1] = '\0';

        printf ("debug: il secondo token che deve essere il pid: \"%s\"\n",token);

        // copio il token nella stringa preparata nel main usando il suo puntatore



        free(stringa_da_verificare);
        return 1;
}

*/
