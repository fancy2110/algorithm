#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define TRUE 1

int build_socket()
{
    int server_socket_fd = 0;
    int server_len = 0;

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(8888);
    server_len = sizeof(server_address);

    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket_fd, (struct sockaddr *)&server_address, server_len);
    listen(server_socket_fd, 5);
    return server_socket_fd;
}

int main(int argc, const char **argv)
{
    int server_socket_fd, client_socket_fd;
    fd_set readfds, testfds;
    int result;

    server_socket_fd = build_socket();

    FD_ZERO(&readfds);
    FD_SET(server_socket_fd, &readfds);

    while (TRUE)
    {
        char ch;
        int fd;
        int nread;
        testfds = readfds;
        printf("Server waiting \n");

        result = select(FD_SETSIZE, &testfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0);
        if (result < 1)
        {
            perror("server strart failed");
            exit(1);
        }

        for (fd = 0; fd < FD_SETSIZE; fd++)
        {
            if (!FD_ISSET(fd, &testfds))
            {
                continue;
            }

            if (fd == server_socket_fd)
            { /*判断是否为服务器套接字，是则表示为客户请求连接。*/
                struct sockaddr_in client_address;
                int client_len = sizeof(client_address);
                client_socket_fd = accept(server_socket_fd, (struct sockaddr *)&client_address, &client_len);
                FD_SET(client_socket_fd, &readfds);
                printf("adding client on fd %d\n", client_socket_fd);
            }
            else
            { /*客户端socket中有数据请求时*/
                ioctl(fd, FIONREAD, &nread);

                if (nread == 0)
                { /*客户数据请求完毕，关闭套接字，从集合中清除相应描述符 */
                    close(fd);
                    FD_CLR(fd, &readfds);
                    printf("removing client on fd %d\n", fd);
                }
                else
                { /*处理客户数据请求*/
                    read(fd, &ch, 1);
                    sleep(5);
                    printf("serving client on fd %d\n", fd);
                    ch++;
                    write(fd, &ch, 1);
                }
            }
        }
    }
    return 0;
}