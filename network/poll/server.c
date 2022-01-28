#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#define TRUE 1
#define CLIENTSIZE 100
#define BUFSIZE 10

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

int setnoblock(int fd)
{
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | O_NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}

int main(int argc, const char **argv)
{
    struct pollfd fds[CLIENTSIZE];

    char data[CLIENTSIZE];
    int server_fd = build_socket();

    fds[0].fd = server_fd;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;
    int conncount = 0;

    while (TRUE)
    {
        int ret = poll(fds, conncount + 1, -1);
        if (ret < 0)
        {
            perror("Server start failed");
            exit(-1);
        }

        for (int i = 0; i < conncount + 1; i++)
        {
            struct pollfd *item = &fds[i];
            // if ((item->revents & POLL_HUP) || (item->revents & POLL_ERR))
            if ((item->revents & POLLHUP))
            {
                int fd = item->fd;
                fds[i] = fds[conncount];
                i--;
                conncount--;
                close(fd);
                printf("delete connection: %d, cause by %d\n", fd, item->revents);
                perror("recieve fail info");
            }
            else if ((item->fd == server_fd) && (item->revents & POLLIN))
            {
                struct sockaddr_in client;
                socklen_t lenaddr = sizeof(client);
                int conn = accept(server_fd, (struct sockaddr *)&client, &lenaddr);
                if (conn == -1)
                {
                    perror("accept client failed");
                    exit(-1);
                }

                printf("get connection %d from %s:%d\n", conn, inet_ntoa(client.sin_addr), client.sin_port);
                conncount++;
                // setnoblock(conn);
                fds[conncount].fd = conn;
                fds[conncount].events = POLLIN | POLLHUP | POLLERR;
                fds[conncount].revents = 0;
            }
            else if (item->revents & POLLIN)
            {
                char ch = 'A';
                int ret = read(item->fd, &ch, 1);
                if (ret < 0)
                {
                    perror("read failed");
                    exit(-1);
                }
                printf("read data from:%d = %c\n", item->fd, ch);
                data[i] = ch + 1;
                item->events &= (~POLLIN);
                item->events |= POLLOUT;
            }
            else if (item->revents & POLLOUT)
            {
                printf("write data to:%d = %c\n", item->fd, data[i]);
                write(item->fd, &data[i], 1);
                item->events &= (~POLLOUT);
                item->events |= POLLIN;
            }
        }
    }
}