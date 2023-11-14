//tcp_server.c 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <string.h> 
#define N 100 
#define PORT 7887
int main(){ 
	int serv_sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	struct sockaddr_in serv_addr; 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	if (bind(serv_sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) 
		return 0; 
	if (listen(serv_sockfd, 1) < 0) 
		return 0; 
	printf("Server listening on port %d.\n", PORT); 

	int client_sockfd = accept(serv_sockfd, NULL, NULL); 
	char buffer_rec[N]; 
	char buffer_send[N]; 

	while(1){ 
		printf("\nSERVER: "); 
		gets(buffer_send); 
		if(strcmp(buffer_send,"exit") == 0){ 
			send(client_sockfd, buffer_send, sizeof(buffer_send), 0); 
			break; 
		}
	       //Logic	
		send(client_sockfd, buffer_send, sizeof(buffer_send), 0);  
		recv(client_sockfd, buffer_rec, sizeof(buffer_send), 0); 
		printf("\nCLIENT: %s\n",buffer_rec);
		if(strcmp(buffer_rec,"exit") == 0){ 
			break; 
		} 
	} 
	close(serv_sockfd); 
	return 0; 
} 
