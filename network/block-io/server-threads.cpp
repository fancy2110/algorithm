#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>


#define MAXLEN 1024

void * sub_service(void * args)
{
    int n;
    char buff[MAXLEN];
    int confd = *(int *)args;

    printf("start a session! %d\n", confd);
    //接收客户端的信息
    while (1) 
    {
        sleep(1);
        n = recv(confd, buff, MAXLEN, 0);
        buff[n] = '\0';
        printf("Thread_%u receive client msg:%s\n", pthread_self(), buff);

        if (strcmp(buff, "exit\n") == 0) {
            printf("End a session! Thread_%u\n",pthread_self());
            break;
            close(confd);
        }
    }
}

int main(int argc, char **argv)
{
    int listenfd, confd;

    struct sockaddr_in sockaddr;
  
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

        pthread_t tid1;
        int new_confd =  confd;
        int err = pthread_create(&tid1, NULL, sub_service, &new_confd);
        if (err != 0) 
        {
            printf("Create thread error");
            close(confd);
        }
    }

    return close(listenfd);
}


