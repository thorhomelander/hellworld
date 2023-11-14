#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define RECEIVER_IP "127.0.0.1"
#define RECEIVER_PORT 6538
#define MAX_PACKET_SIZE 100

int sockfd;
struct sockaddr_in receiverAddr;

void send_packet(int seq_num, int data_start, int data_end) {
	char packet_data[MAX_PACKET_SIZE];
	sprintf(packet_data, "SEQ=%d, DATA=%d-%d", seq_num, data_start, data_end);

	sendto(sockfd, packet_data, strlen(packet_data), 0, (struct sockaddr*)&receiverAddr, sizeof(receiverAddr));
}

int main() {
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&receiverAddr, 0, sizeof(receiverAddr));
	receiverAddr.sin_family = AF_INET;
	receiverAddr.sin_port = htons(RECEIVER_PORT);
	inet_pton(AF_INET, RECEIVER_IP, &receiverAddr.sin_addr);

	int n = 2500;
	int MSS = 100;
	int ISN = 0;
	int m = 600;
	int i = 50;
	int j = 200;
	int k = 5;
	int last_byte_sent = ISN - 1;
	int last_byte_acknowledged = ISN - 1;
	int effective_window = m;

	while (effective_window > 0 && last_byte_sent < n) {
		int data_start = last_byte_sent + 1;
		int data_end = data_start + MSS - 1;

		send_packet(data_start, data_start, data_end);

		last_byte_sent = data_end;

		char ack_data[MAX_PACKET_SIZE];
		recvfrom(sockfd, ack_data, sizeof(ack_data), 0, NULL, NULL);
		int ack_num, window;
		sscanf(ack_data, "ACK=%d, AW=%d", &ack_num, &window);

		last_byte_acknowledged = ack_num;
		effective_window = window;

		printf("Sent: SEQ=%d, ACK=%d, AW=%d\n", data_start, ack_num, effective_window);
	}

	close(sockfd);

	return 0;
}

