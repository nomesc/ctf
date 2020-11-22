#include <arpa/inet.h>
#include <stdio.h>

#include "client.h"

int main()
{
    char * LOCALHOST = "127.0.0.1";
    int PORT = 56647;
    struct sockaddr_in serv_addr;
    int ret = -1;

    int socket_fd = -1;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        puts("socket creation failed");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, LOCALHOST, &serv_addr.sin_addr) <= 0)
    {
        puts("serv_addr creation failed");
        return -1;
    }

    struct client * clients = mallloc(100 * sizeof(struct client));
    for (int i = 0; i < 100; i++)
    {
        
    }

    return 0;
}