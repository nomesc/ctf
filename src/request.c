#include "request.h"
#include "service_flag.h"
#include "service_population.h"

void *dispatch(void *arg)
{
    int BUFF_SIZE = 128;
    uint32_t OK = 0;
    char *buffer = malloc(BUFF_SIZE);
    struct request *request = (struct request *)arg;

    int ret = read(request->client_connection, buffer, BUFF_SIZE);
    if (ret == -1)
    {
        ERROR("Could not recieve data from client");
        free(buffer);
        return NULL;
    }
    buffer[ret] = '\0';

    do
    {
        // call service handler
        send(request->client_connection, "HELLO!\n", 8, 0);
        read(request->client_connection, buffer, BUFF_SIZE);
        if (strncmp(buffer, "ACK", 3) == 0)
        {
            OK = 1;
        }
    } while (OK != 1);

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
    return NULL;
}