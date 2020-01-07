#include "format.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
int readMessage(char *Buff, message* _message);
int	createMessage(message m,char *Buff);

int readMessage(char *Buff, message* _message)
{
    char *rawBuff,*found;
    rawBuff = strdup(Buff);
    found = strsep(&rawBuff,SPLITSYMBOL);
    //printf("%s\n",found);
    _message->message_type = found[0];
    found = strsep(&rawBuff,SPLITSYMBOL);
    //printf("%s\n",found);
    _message->student_id = found;
    found = strsep(&rawBuff,SPLITSYMBOL);
    //printf("%s\n",found);
    _message->message_data = found;

    return 0;
}

int	createMessage(message m,char *Buff)
{
	char type[1];
    type[0] = m.message_type;
    char *tBuff = (char*) calloc(MAX_SIZE_MESSAGE, sizeof(char));
   // printf("tBuff 1: %s\n",tBuff);
	strcat(tBuff,type);
	//printf("tBuff 2: %s\n",tBuff);
	strcat(tBuff,SPLITSYMBOL);
	//printf("tBuff 3: %s\n",tBuff);
	strcat(tBuff,m.student_id);
	//printf("tBuff 4: %s\n",tBuff);
	strcat(tBuff,SPLITSYMBOL);
	//printf("tBuff 5: %s\n",tBuff);
	strcat(tBuff,m.message_data);
	//printf("tBuff 6: %s\n",tBuff);
	strcpy(Buff,tBuff);
	free(tBuff);
	return 0;
}