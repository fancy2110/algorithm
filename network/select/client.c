#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

int main(int argc, const char **argv)
{
    int client_sockfd;
    int len;

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(8888);

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int result = connect(client_sockfd, (struct sockaddr *)&address, sizeof(address));

    if (result == -1)
    {
        perror("Client start failed");
        exit(-1);
    }

    char ch = 'A';
    sleep(5);
    int send_ret = write(client_sockfd, &ch, 1);
    if (send_ret < 0)
    {
        perror("send failed");
        exit(-1);
    }

    sleep(5);
    int read_ret = read(client_sockfd, &ch, 1);
    if (read_ret < 0)
    {
        perror("read_failed");
        exit(-1);
    }
    printf("th first time : char from server = %c\n", ch);
    sleep(5);

    int ret = write(client_sockfd, &ch, 1);
    if (ret < 0)
    {
        perror("send failed");
        exit(-1);
    }
    read(client_sockfd, &ch, 1);
    printf("the second time: char from server = %c\n", ch);
    sleep(5);
    close(client_sockfd);
    return 0;
}