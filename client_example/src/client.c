#include <string.h>

#include "client.h"
#include "command.h"

int custom_command(struct client * client, int server_socket)
{
    dispatch_command(
        server_socket,
        client->buff,
        client->buff_size,
        client->command
    );
}

int acknowledge_command(struct client * client, int server_socket)
{
    dispatch_command(
        server_socket,
        client->buff,
        client->buff_size,
        "ACK"
    );
}

char * get_raspuns_primit(struct client * client)
{
    return client->raspuns_primit;
}

int refresh_raspuns(struct client * client, int server_socket, char * command)
{
    dispatch_command(
        server_socket,
        client->buff,
        client->buff_size,
        command
    );   
}