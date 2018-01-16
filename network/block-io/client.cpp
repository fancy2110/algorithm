#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define MAXLEN 1024

int main(int argc, char **argv)
{
    char * server_inet_addr = "127.0.0.1";
    int port = 4444;
    int sockfd;
    struct sockaddr_in sockaddr;
    char recv_buff[MAXLEN], send_buff[MAXLEN];
    int n;


    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(4444);
    inet_pton(AF_INET, server_inet_addr, &sockaddr.sin_addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(struct sockaddr)) < 0) 
    {
        printf("connect error %s errno: %d\n",strerror(errno),errno);  
        return 0;
    }

    while(1) 
    {
        printf("Please input msg:");

        fgets(send_buff, MAXLEN, stdin);

        if (send(sockfd, send_buff, strlen(send_buff), 0) < 0)
        {
            printf("send mes error: %s errno : %d",strerror(errno),errno);
            break;
        }

        printf("Message send:%s", send_buff);

        if (strcmp(send_buff, "exit\n") == 0 || strcmp(send_buff, "close\n") == 0)
        {
            printf("exit!\n");
            break;
        }

        memset(send_buff, 0, MAXLEN);
    }

    close(sockfd);
}