#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>


int main()
{

     char *ip="127.0.0.1";
     int port=1200;

     int server_sock,client_sock;
     struct sockaddr_in server_addr,client_addr;
     socklen_t addr_size;
     char buffer[35024];
     int n;

     server_sock=socket(AF_INET,SOCK_STREAM,0);

     if(server_sock < 0)
     {
              perror("socket error\n");
              exit(1);
     }
  printf("socket created\n");
     memset(&server_addr,'\0',sizeof(server_addr));
     server_addr.sin_family=AF_INET;
     server_addr.sin_port=port;
     server_addr.sin_addr.s_addr=inet_addr(ip);

     n=bind(server_sock,(struct sockaddr*) &server_addr,sizeof(server_addr));

        if(n<0)
        {
          perror("bind error\n");
           exit(1);
        }
      listen(server_sock,1);
      printf("listening on %d port\n",port);
        while(1)
        {
            addr_size=sizeof(client_addr);
            client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&addr_size);
            printf("connected to client\n");

            bzero(buffer,35024);
            recv(client_sock,buffer,sizeof(buffer),0);

             FILE *fp;
            int i=0;
            char c;
            fp=fopen(buffer,"r");
            bzero(buffer,35024);

             char buffer2[30000];
            bzero(buffer,35024);
            fscanf(fp,"%s",buffer);
            send(client_sock,buffer,sizeof(buffer),0);

        }

     return 0;
}
