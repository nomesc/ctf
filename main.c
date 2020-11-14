#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include "globals.h"

int main()
{
    int socket_fd = -1;
    int new_con = -1;
    int ret = -1;
    int opt = 1;
    struct sockaddr_in address = {0};
    socklen_t address_len = sizeof(address);

    // Creating socket file descriptor
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        ERROR("Could not create socket file descriptor");
        return -1;
    }

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret == -1)
    {
        ERROR("Could not set reuse option");
        return -1;
    }

    // Binding socket to address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    ret = bind(socket_fd, (struct sockaddr *)&address, address_len);
    if (ret == -1)
    {
        ERROR("Could not bind to port");
        return -1;
    }

    // Start listening to port
    ret = listen(socket_fd, CONN_QUEUE_SIZE);
    if (ret == -1)
    {
        ERROR("Could not listen on socket");
        return -1;
    }

    while (1)
    {
        puts("Asteptam clientul");
        new_con = accept(socket_fd, (struct sockaddr *)&address, &address_len);
        if (new_con == -1)
        {
            ERROR("Could not open new connection with the client");
            continue;
        }
        puts("Asteptam Date");
        ret = read(new_con, g_buffer, 1024);
        if (ret == -1)
        {
            ERROR("Could not recieve data from client");
            continue;
        }
        g_buffer[ret] = '\0';
        printf("%s\n", g_buffer);
        puts("Trimitem date");
        send(new_con, "HELLO!\n", 8, 0);
        puts("Am trimis mesajul");

        // Closing connection
        ret = shutdown(new_con, SHUT_WR);
        if (ret == -1)
        {
            ERROR("Could not shutdown client connection");
        }
        do
        {
            ret = recv(new_con, g_buffer, 1024, MSG_WAITALL);
        } while (ret != 0 || (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK));
        close(new_con);
        if (ret == -1)
        {
            ERROR("Could not close client connection descriptor");
        }
        puts("Am terminat");
    }
    return 0;
}