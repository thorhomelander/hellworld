#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#define N 100
#define PORT 9345

int main()
{
        int serv_sockfd = socket(AF_INET,SOCK_STREAM,0);
        struct sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        if(bind(serv_sockfd,(struct sockaddr* )&serv_addr, sizeof(serv_addr)) <0)
                return 0;
        if(listen(serv_sockfd, 1)<0)
                return 0;
        printf("server listening on port -%d\n", PORT);
        int client_sockfd = accept(serv_sockfd,NULL,NULL);
        char buffer_rec[N],input[N];
        char buffer_send[N];
        while(1)
        {
                recv(client_sockfd,buffer_rec,sizeof(buffer_rec),0);
                int i,num1=0,num2=0;
                int result=0;
                char op;
                for(i=0;buffer_rec[i] != '\0';i++)
                {
                        if(buffer_rec[i] >= '0' && buffer_rec[i]<='9')
                                num1 = num1*10 + (buffer_rec[i] - '0');
                        else
                        {
                                op=buffer_rec[i];
                                break;
                        }
                }
                i++;
                while(buffer_rec[i] != '\0')
                {
                        if(buffer_rec[i] >= '0' && buffer_rec[i]<='9')
                                num2 = num2*10 + (buffer_rec[i] - '0');
                        i++;
                }
                if(op == '+')
                        result=(num1 + num2);
                else if(op == '-')
                        result= num1-num2;
                else if(op == '*')
                        result=num1*num2;
                else if(op == '/')
                        result = num1/num2;
                char a[10],b[10];
                i=0;
                while(result > 0)
                {
                        a[i]=result%10 + '0';
                        i++;
                        result /= 10;
                }
                for(int j=0;j<i;j++)
                {
                        b[j]=a[i-1-j];
                }
                b[i]='\0';
                printf("\nCLIENT :  %s\n",buffer_rec);
                printf("\nSERVER :  %s\n",b);
                send(client_sockfd,b,sizeof(b),0);

        }
        close(serv_sockfd);
        return 0;
}

