#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAXLEN 1024
int fds[MAXLEN] = {0};
int fds_count = 0;
int max_sock_fd = 0;
fd_set rfds;

int main(int argc, char **argv)
{
    int listenfd, confd;

    struct sockaddr_in sockaddr;
    char buff[MAXLEN];
    int n ;
    int nOptVal = 1;

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;

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
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &nOptVal, sizeof(int)) == -1)
    {
        return -2;
    } 
    /**
     * 将socket 文件符绑定到socketaddr上
    */
    bind(listenfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr));

    listen(listenfd, 1024);

    FD_ZERO(&rfds);
    max_sock_fd = listenfd;
    fds_count = 1;

    printf("Server start, waiting client information...\n");
    while(1) 
    {
        FD_SET(listenfd, &rfds);
        for(int i=0; i < fds_count; i++)
        {
            if (fds[i] == 0)
            {
                continue;
            }

            FD_SET(fds[i], &rfds);
        }


        int ret = select(max_sock_fd + 1, &rfds, NULL, NULL, &tv);
        if (ret < 0)
        {
            printf("select failed!\n");
            break;
        } 
        else if (ret == 0)
        {
            printf("select timeout: %d!\n", max_sock_fd);
            continue;
        }

        if (FD_ISSET(listenfd, &rfds))
        {
            //开始等待客户端的连接
            if ((confd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) 
            {
                printf("accpet socket error: %s errno :%d\n",strerror(errno), errno);
                continue;
            }

            if (fds_count < MAXLEN)
            {
                for(int i=0; i < MAXLEN; i++)
                {
                    if (fds[i] == 0) {
                        printf("Start a new session:%u, at %d\n", confd, i);
                        fds[i] = confd;
                        FD_SET(fds[i], &rfds);
                        fds_count++;
                        break;
                    }
                }

                if (confd > max_sock_fd) 
                {
                    max_sock_fd = confd;
                }
            }            

            if ( --ret <= 0 ) 
            {
                continue;
            }
        }

        for (int i=0; i < fds_count; i++) 
        {
            if (fds[i] > 0 && FD_ISSET(fds[i], &rfds))
            {
                //接收客户端的信息
                n = recv(fds[i], buff, MAXLEN, 0);
                if (n <= 0)
                {
                    continue;
                }

                buff[n] = '\0'; 
                printf("Receive client msg:%s", buff); 

                if (strcmp(buff, "exit\n") == 0) {
                    printf("End a session!\n");
                    FD_CLR(fds[i], &rfds);
                    close(fds[i]);
                    fds[i] = 0;
                    fds_count --;
                }

                if (--ret <= 0) break;
            }
        }

        sleep(1);
    }

    return close(listenfd);
}

