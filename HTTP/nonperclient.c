# include "myheader.h"

int main(int argc, char *argv[]){

	request cr;
	strcpy(cr.method, "GET");
	strcpy(cr.path, "/");
	strcpy(cr.version, "http/1.1");
	strcpy(cr.connection, "keep-alive");
	strcpy(cr.accept, "text");

	struct sockaddr_in cliaddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(PORT);
	cliaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0) 
		return 0;
	printf("Connection made with the server.\n");

	printf("Enter useragent: ");
	scanf("%s", cr.useragent);
	printf("Enter RegNo of the student: ");
	scanf("%d", &cr.stu.regno);

	while(1){
		printf("Enter semester [-1 to exit]: ");
		scanf("%d", &cr.stu.sem);

		send(sockfd, &cr, sizeof(cr), 0);

		if (cr.stu.sem == -1) break;

		response sr;
		int len = recv(sockfd, &sr, sizeof(sr), 0);
		if (len > 0){
			printf("SERVER'S RESPONSE: \n%s\n%d%s\nConnection: %s\nContent-Type: %s\n\n", sr.version, sr.status,sr.statmsg, sr.connection, sr.contype);
			if (sr.status == 404){
				printf("Marksheet not found.\n");
			}
			else{
				printf(" MARKSHEET\n");
				printf("|-----------------------------|\n");
				printf("|REGNO: NAME:\t AGE: GENDER:|\n");
				printf("|%d %s\t %d %c |\n",sr.stu.regno, sr.stu.name, sr.stu.age, sr.stu.gender);
				printf("|-----------------------------|\n");
				printf("|SEM: %d\tMARKS: ", sr.stu.sem);
				for (int j = 0; j < 5; j++){
					printf("%d ", sr.stu.marks[j]);
				}
				printf("|\n|-----------------------------|\n");
			}
		}
	}
	close(sockfd);
	printf("Disconnected from the server.\n");

	return 0;
}
