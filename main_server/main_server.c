#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <pthread.h> 
#define PORT 45000
#define MAX_CLIENT 2
#define MAX_SIZE_MESSAGE 1024 // byte

extern void handle_request(int sock, char *Buff);
void *thread_serve_func(void *thread_sock);
int _clientAlive = 0;

int main(int argc, char const *argv[]) 
{ 

	// 	CREATE SOCKET
	printf("SERVER_RUN...\n");
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[MAX_SIZE_MESSAGE] = {0}; 

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("CREATESOCKET: socket()...\n");   
    // Forcefully attaching socket to the port 45000 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    printf("CREATESOCKET: setsockopt()...\n");   
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 45000 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("CREATESOCKET: bind()...\n");   
    if (listen(server_fd, MAX_CLIENT) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    printf("CREATESOCKET: listen()...\n");   

    // SETUP LOOP TO SERVE
    printf("**********************************\n");   
    printf("SERVE: Wait for connection...\n");   
    while( (new_socket = accept(server_fd, (struct sockaddr*)NULL, NULL)))
    {
    	int *thread_sock = (int*) malloc(sizeof(int));
		*thread_sock = new_socket;
    	_clientAlive = _clientAlive+1;
        if(_clientAlive > MAX_CLIENT){
            printf("SERVE: Over MAX_CLIENT, Reject connection...\n");   
            free(thread_sock); 
        }
        else
        {
        	printf("SERVE: Connection accepted: Client: %d, new_socket: %d\n", _clientAlive , new_socket);   
	        pthread_t serve_thread;

	        if( pthread_create(&serve_thread , NULL , thread_serve_func , (void*)thread_sock)) 
	        {
	            printf("SERVE: can not create thread...\n");   
	            exit(EXIT_FAILURE); 
	        }
        };
        
    }
    
    if (new_socket == 0)
    {
        printf("SERVE: Fail new_socket...\n");   
        exit(EXIT_FAILURE); 
    }


    return 0; 
}

void *thread_serve_func(void *thread_sock)
{
    //Get the socket descriptor
    int sock = *(int*)thread_sock;
    printf("--------------------------\n"); 
    printf("NEW CONNECTION: Thread in socket = %d...\n",sock);
    printf("--------------------------\n");    
    int readByte;
    char recvBuff[MAX_SIZE_MESSAGE];

    while ( (readByte = read(sock, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[readByte] = 0;  
        handle_request(sock, recvBuff);
    }

    free(thread_sock);
    return 0;
}

