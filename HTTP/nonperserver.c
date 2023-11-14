#include "myheader.h"

int main(int argc, char *argv[]){
	response sr;
	strcpy(sr.version, "http/1.1");
	strcpy(sr.connection, "keep-alive");
	strcpy(sr.contype, "text");
	struct sockaddr_in serveraddr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(sockfd, (struct sockaddr *) &serveraddr,
				sizeof(serveraddr)) < 0) return 0;
	if (listen(sockfd, 2) < 0) return 0;
	printf("Server listening on port: %d\n", PORT);

	int clisock = accept(sockfd, NULL, NULL);
	if (clisock < 0) return 0;

	while(1){ 
		request clr;
		int len = recv(clisock, &clr, sizeof(clr), 0);
		printf("CLIENT'S REQUEST:\n%s%s%s\nConnection: %s\nAccept: %s\nUser-Agent: %s\nRegno: %d\nRequired sem: %d\n\n",clr.method, clr.path, clr.version,clr.connection, clr.accept, clr.useragent, clr.stu.regno, clr.stu.sem);
		if (clr.stu.sem == -1) break;

		FILE *inf;
		inf = fopen("stud.txt", "rb");
		student s;
		int found = 0;
		while(fread(&s, sizeof(student), 1, inf) == 1){
			if (s.regno == clr.stu.regno && s.sem ==
					clr.stu.sem) {
				found = 1;
				sr.stu = s;
				strcpy(sr.statmsg, "OK");
				sr.status = 200;
				break;
			}
		} if (!found){
			sr.stu.regno = -1;
			sr.stu.sem = -1;
			strcpy(sr.statmsg, "Not found");
			sr.status = 404;
		}
		fclose(inf);
		send(clisock, &sr, sizeof(sr), 0);
	}
	close(sockfd);
	return 0;
}
