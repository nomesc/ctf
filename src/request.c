#include "request.h"

void dispatch(struct request *request)
{
    int BUFF_SIZE =  128;
    char * buffer = malloc(BUFF_SIZE);

    int ret = read(request->client_connection, buffer, BUFF_SIZE);
    if (ret == -1)
    {
        ERROR("Could not recieve data from client");
        free(buffer);
        return;
    }
    buffer[ret] = '\0';
    
    send(request->client_connection, "HELLO!\n", 8, 0);

    // Closing connection
    ret = shutdown(request->client_connection, SHUT_WR);
    if (ret == -1)
    {
        ERROR("Could not shutdown client connection");
    }
    do
    {
        ret = recv(request->client_connection, buffer, BUFF_SIZE, MSG_WAITALL);
    } while (ret != 0 || (ret == -1 && errno != EAGAIN && errno != EWOULDBLOCK));
    close(request->client_connection);
    if (ret == -1)
    {
        ERROR("Could not close client connection descriptor");
    }

    free(buffer);
}