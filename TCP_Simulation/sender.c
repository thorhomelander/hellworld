#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define RECEIVER_IP "127.0.0.1"
#define RECEIVER_PORT 9636
#define MAX_PACKET_SIZE 100

int sockfd;
struct sockaddr_in receiverAddr;
int n, MSS, ISN, m, i, j, k, congestion_window,last_byte_sent,last_byte_acknowledged,advertized_window, effective_window;
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
	int no = 1;
	n = 2500;
	MSS = 100;
	ISN = 0;
	m = 2000;
	i = 1;
	j = 500;
	k = 10;
	int non_receipt = 0;
	congestion_window = 100;
	last_byte_sent = ISN - 1;
	last_byte_acknowledged = ISN - 1;
	advertized_window = m;
	printf("Effective Window = %d, Congestion Window = %d\n", effective_window, congestion_window);
	while (advertized_window > 0 && last_byte_sent < n - 1) {
		int data_start = last_byte_sent + 1;
		int data_end = data_start + MSS - 1;

		send_packet(data_start, data_start, data_end);

		last_byte_sent = data_end;

		char ack_data[MAX_PACKET_SIZE];
		recvfrom(sockfd, ack_data, sizeof(ack_data), 0, NULL, NULL);
		int ack_num, window;
		if(k != -1)
			k -= 1;
		sscanf(ack_data, "ACK=%d, AW=%d", &ack_num, &window);
		if(last_byte_acknowledged  + 1 != ack_num){
			last_byte_acknowledged = ack_num - 1;
			advertized_window = window;
			window = congestion_window > advertized_window ? advertized_window : congestion_window;
			effective_window = window - (last_byte_sent - last_byte_acknowledged);
			congestion_window *= 2;
		}else{
			if(non_receipt == 0){
				non_receipt = 1;
				congestion_window /= 2;
				last_byte_acknowledged = ack_num - 1;
				advertized_window = window;
				window = congestion_window > advertized_window ? advertized_window : congestion_window;
				effective_window = window - (last_byte_sent - last_byte_acknowledged);
			}
			else if(non_receipt == 1){
				int increment = (MSS * MSS) / congestion_window;
				congestion_window += increment;
				last_byte_acknowledged = ack_num - 1;
				advertized_window = window;
				window = congestion_window > advertized_window ? advertized_window : congestion_window;
				effective_window = window - (last_byte_sent - last_byte_acknowledged);
			}
		}
		if(k != 0){
			printf("%d\nSent: SEQ=%d, ACK=%d, AW=%d\n",no, data_start, ack_num, advertized_window);
			printf("Effective Window = %d, Congestion Window = %d\n", effective_window, congestion_window);
		}
		no++;
	}

	close(sockfd);

	return 0;
}
