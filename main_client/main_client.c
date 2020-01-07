#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <stdlib.h>
#include "format.h"
#define PORT 45000

// extern from helper.c
extern int readMessage(char *Buff, message* _message);
extern int	createMessage(message m,char *Buff);

int main(int argc, char const *argv[]) 
{ 
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("Socket creation error \n"); 
		return -1; 
	} 

	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("Invalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("Connection Failed \n"); 
		return -1; 
	} 

	printf("Student score search...\n");


	int choose = 1;
	char *id = (char*) calloc(100, sizeof(char));
	char *passwd = (char*) calloc(100, sizeof(char));
	// menu
	while(choose)
	{
		printf("*************************\n");
		printf("1. log in\n");
		printf("2. log out\n");
		printf("3. score request\n");
		printf("0. quit\n");
		printf("Type your choose: ");
		scanf("%d", &choose);
		fflush(stdin);
		message req;
		switch(choose)
		{
			case 1:
				printf("id: ");
				scanf("%s",id);
				fflush(stdin);
				printf("password: ");
				scanf("%s",passwd);
				fflush(stdin);
				req.message_type = MESSAGE_LOG_IN_REQUEST;
				break;
			case 2:
				req.message_type = MESSAGE_LOG_OUT_REQUEST ;
				break;
			case 3:
				req.message_type = MESSAGE_SCORE_REQUEST ;
				break;
			default: 
				continue;
		}
		req.student_id = id;
		req.message_data = passwd;
		// Send request message
		char *requestBuff = (char*) calloc(MAX_SIZE_MESSAGE, sizeof(char));
		if(createMessage(req,requestBuff)!= 0)
		{
			printf("Cannot create message %s...\n",requestBuff); 
			return 1;
		}
		//printf("You send: %s\n",requestBuff);
		send(sock , requestBuff , strlen(requestBuff) , 0 ); 
		free(requestBuff);

		// Recv response message
		char *responseBuff = (char*) calloc(MAX_SIZE_MESSAGE, sizeof(char));
		valread = read( sock , responseBuff, MAX_SIZE_MESSAGE); 
		responseBuff[valread] = 0;
		//printf("You recv: %s\n",responseBuff);
		message reponseMess;
		if(readMessage(responseBuff,&reponseMess)!= 0)
		{
			printf("receive fail message %s...\n",responseBuff); 
			return 1;
		}
		free(responseBuff);
		switch(reponseMess.message_type)
		{
			case MESSAGE_LOG_IN_SUCCESS:
				printf("log in successfully.\n");
				break;
			case MESSAGE_LOG_OUT_SUCCESS:
				printf("log out successfully.\n");
				break;
			case MESSAGE_LOG_IN_FAIL:
				printf("log in fail.\n");
				break;
			case MESSAGE_SCORE_FAIL:
				printf("fail to get score.\n");
				break;
			case MESSAGE_SCORE_SUCCESS:
				printf("score: %s\n",reponseMess.message_data);
				break;
		}	
	}
	free(id);
	free(passwd);
	printf("Exit...\n");

	return 0; 
} 


