#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

#define MAXLEN 1024
int main(int argc, char **argv)
{
    int listenfd, confd;

    struct sockaddr_in sockaddr;
    char buff[MAXLEN];
    int n ;

    /**
     * 初始化socket 协议参数以及监听端口
     * **/
    memset(&sockaddr, 0 , sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(4444);

    /***
     * 创建socket嵌套字节文件符
     **/
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /**
     * 将socket 文件符绑定到socketaddr上
    */
    bind(listenfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

    listen(listenfd, 1024);
    printf("Server start, waiting client information...\n");

    while(1) 
    {
        //开始等待客户端的连接
        if ((confd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) 
        {
            printf("accpet socket error: %s errno :%d\n",strerror(errno), errno);
            continue;
        }

        //设置sockfd为非阻塞io模式
        int flags = fcntl(confd, F_GETFL, 0);
        fcntl(confd, F_SETFL, flags | O_NONBLOCK);

        //接收客户端的信息
        while (1) 
        {
            n = recv(confd, buff, MAXLEN, 0);
            if (n < 0) 
            {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    continue;
                } else {
                    printf("read socket error: %s errno :%d\n",strerror(errno), errno);
                }
            }

            buff[n] = '\0'; 
            printf("Receive client msg:%s", buff); 

            if (strcmp(buff, "exit\n") == 0) {
                printf("End a session!\n");
                break;
            }

            if (strcmp(buff, "close\n") == 0) {
                printf("End a session!\n");
                break;
            }
        }

        if (strcmp(buff, "close\n") == 0) {
            printf("Stop server!\n");
            break;
        }

        fcntl(confd, F_SETFL, flags);

        close(confd);
    }

    return close(listenfd);
}