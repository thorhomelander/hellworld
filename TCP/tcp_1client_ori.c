//tcp_client.c 
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
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	struct sockaddr_in serv_addr; 
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	serv_addr.sin_addr.s_addr = INADDR_ANY; 
	int status = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)); 
	if (status == -1){ 
		printf("Error in making the connection\n"); 
		return 0; 
	} 
	printf("Connection established!\n"); 

	char buffer_rec[N]; 
	char buffer_send[N]; 
	while(1){ 
		recv(sockfd, buffer_rec, sizeof(buffer_send), 0); 
		printf("\nSERVER: %s\n",buffer_rec); 
		if(strcmp(buffer_rec,"exit") == 0){ 
			break; 
		} 
		printf("\nCLIENT: "); 
		gets(buffer_send); 
		if(strcmp(buffer_send,"exit") == 0){ 
			send(sockfd, buffer_send, sizeof(buffer_send), 0); 
			break; 
		} 
		send(sockfd, buffer_send, sizeof(buffer_send), 0); 
//		recv(sockfd, buffer_rec, sizeof(buffer_send), 0);
//		printf("\nSERVER: %s\n",buffer_rec);
	} 
	close(sockfd); 
	return 0; 
}
