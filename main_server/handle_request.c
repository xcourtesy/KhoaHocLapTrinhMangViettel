#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include "format.h"


#define  CLIENT_DATABASE_SIZE 5


// extern from helper.c
extern int readMessage(char *Buff, message* _message);
extern int	createMessage(message m,char *Buff);

// data

student _student[CLIENT_DATABASE_SIZE] = {
	{"Luu15ac3"	,"Calc22@"			,7.888		,0},
	{"Huy479"	,"C"				,10			,0},
	{"Lan211"	,"0466"				,8.0		,0},
	{"TuAnh098"	,"atk3"				,4.6		,0},
	{"Yen266a7"	,"99999999999999"	,3			,0},
};



int handle_request(int socket, char *Buff)
{
	printf("***HANDLE NEW REQUEST FROM THREAD SOCKET %d ...\n",socket); 

	// Read and check valid message from client
	printf("THREAD SOCKET %d: Recv message: %s\n",socket,Buff);
	message mess;
	if(readMessage(Buff,&mess)!= 0)
	{
		printf("THREAD SOCKET %d: Receive fail message %s...\n",socket,Buff); 
		return 1;
	}

	// Handle
	// Find client in database
	int findClientIndex = -1;
	int statusResponse = 0; 
	for(int i = 0;i < CLIENT_DATABASE_SIZE;i++)
	{
		if(strcmp(mess.student_id,_student[i].student_id) == 0 &&
			strcmp(mess.message_data,_student[i].student_pass) == 0)
		{
			findClientIndex = i;
			statusResponse = 1;
			break;
		}
	}
	if(statusResponse==1)
	{
		printf("THREAD SOCKET %d: Valid client: %s\n",socket,mess.student_id);
	}
	else
	{
		printf("THREAD SOCKET %d: Invalid client\n",socket);
	}
	
	message response;
	response.student_id = mess.student_id;
	response.message_data = "";
	char *mark = (char*) calloc(5,sizeof(char)); // mark is char array "xx.xx" eg: "07.44"
	// Create response message
	switch(mess.message_type)
	{
		case MESSAGE_LOG_IN_REQUEST:
			if(statusResponse == 1)
			{
				_student[findClientIndex].student_status = 1;
				response.message_type = MESSAGE_LOG_IN_SUCCESS;
			}
			else
			{
				response.message_type = MESSAGE_LOG_IN_FAIL;
			}
			break;
		case MESSAGE_LOG_OUT_REQUEST:
			if(statusResponse == 1)
			{
				_student[findClientIndex].student_status = 0;
				response.message_type = MESSAGE_LOG_OUT_SUCCESS;
			}
			break;
		case MESSAGE_SCORE_REQUEST:
			if(statusResponse == 1 && _student[findClientIndex].student_status == 1)
			{
				
				int s1 = (int)(_student[findClientIndex].student_mark/10);
				int s2 = (int)(_student[findClientIndex].student_mark - s1*10);
				int s3 = (int)(_student[findClientIndex].student_mark*10 - s1*100 - s2*10);
				int s4 = (int)(_student[findClientIndex].student_mark*100 - s1*1000 - s2*100 - s3*10);
				mark[0] = s1 + 48; 
				mark[1] = s2 + 48;
				mark[2] = '.';
				mark[3] = s3 + 48;
				mark[4] = s4 + 48;
				response.message_type = MESSAGE_SCORE_SUCCESS;
				response.message_data = mark;
				
			}
			else
			{
				response.message_type = MESSAGE_SCORE_FAIL;
			}
			break;

	}


	// Send response message
	char *responseBuff = (char*) calloc(MAX_SIZE_MESSAGE,sizeof(char));
	if(createMessage(response,responseBuff)!= 0)
	{
		printf("THREAD SOCKET %d: Cannot create response message...\n",socket); 
		return 1;
	}
	printf("THREAD SOCKET %d: Response message: %s\n",socket,responseBuff);
	send(socket , responseBuff , strlen(responseBuff) , 0 ); 
	free(responseBuff);
	free(mark);
	return 0;
}


