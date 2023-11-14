#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RECEIVER_PORT 6538
#define MAX_PACKET_SIZE 100

int sockfd;
struct sockaddr_in serverAddr, clientAddr;
socklen_t addr_len = sizeof(clientAddr);

void process_packet(char* packet_data, int* next_expected_byte, int* advertised_window) {
	int seq_num, data_start, data_end;
	sscanf(packet_data, "SEQ=%d, DATA=%d-%d", &seq_num, &data_start, &data_end);

	if (data_start == *next_expected_byte) {
		*next_expected_byte = data_end + 1;
		//printf("\n%d\n", *next_expected_byte);
		*advertised_window -= (data_end - data_start + 1);
	} else {
		// Out-of-sequence packet
	}
}

int main() {
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(RECEIVER_PORT);

	if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	int m = 600;
	int i = 50;
	int j = 200;
	int k = 5;
	int advertised_window = m;
	int next_expected_byte = 0;
	char ack_data[MAX_PACKET_SIZE];
	sprintf(ack_data, "ACK=%d, AW=%d", -1, advertised_window);
	sendto(sockfd, ack_data, strlen(ack_data), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

	while (advertised_window > 0) {
		char packet_data[MAX_PACKET_SIZE];
		int recv_bytes = recvfrom(sockfd, packet_data, sizeof(packet_data), 0, (struct sockaddr*)&clientAddr, &addr_len);
		if (recv_bytes < 0) {
			perror("Packet receive error");
			continue;
		}

		packet_data[recv_bytes] = '\0';

		process_packet(packet_data, &next_expected_byte, &advertised_window);

		sprintf(ack_data, "ACK=%d, AW=%d", next_expected_byte, advertised_window);
		sendto(sockfd, ack_data, strlen(ack_data), 0, (struct sockaddr*)&clientAddr, sizeof(clientAddr));

		printf("Received: %s, ACK=%d, AW=%d\n", packet_data, next_expected_byte, advertised_window);
	}

	close(sockfd);

	return 0;
}
