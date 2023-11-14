#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.200"
#define PORT 2008

void receiveFile(int serverSocket, char* filename) {
    long fileSize;
    recv(serverSocket, &fileSize, sizeof(long), 0);

    if (fileSize == -1) {
        printf("File not found on server\n");
        return;
    }

    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error creating file");
        return;
    }

    char buffer[1024];
    while (fileSize > 0) {
        int bytesRead = recv(serverSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) break;
        fwrite(buffer, 1, bytesRead, file);
        fileSize -= bytesRead;
    }

    fclose(file);
    printf("File received successfully\n");
}

int main() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        return -1;
    }

    char filename[256];
    printf("Enter the filename: ");
    scanf("%s", filename);

    send(clientSocket, filename, sizeof(filename), 0);

    receiveFile(clientSocket, filename);

    close(clientSocket);
    return 0;
}