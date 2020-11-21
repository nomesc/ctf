#include "request.h"
#include "service_flag.h"
#include "service_population.h"

int populate_request(struct request *req, char *msg)
{
    if (strncmp(msg, "FLG", 3) == 0)
    {
        req->callback_function = service_flag_cb;
        req->handler_function = handle_service_flag;
        req->service = malloc(sizeof(struct service_flag));
        ((struct service_flag *)(req->service))->country_name = malloc(10);
        ((struct service_flag *)(req->service))->color = 1;
        return 0;
    }
    else if (strncmp(msg, "POP", 3) == 0)
    {
        req->callback_function = service_population_cb;
        req->handler_function = handle_service_population;
        req->service = malloc(sizeof(struct service_population));
        ((struct service_population *)(req->service))->country_name = malloc(10);
        return 0;
    }
    else
    {
        return -1;
    }
}

void *dispatch(void *arg)
{
    int BUFF_SIZE = 128;
    int ret = -1;
    uint32_t OK = 0;
    char *buffer = malloc(BUFF_SIZE);
    struct request *request = (struct request *)arg;

    ret = read(request->client_connection, buffer, BUFF_SIZE);
    if (ret == -1)
    {
        ERROR("Could not recieve data from client");
        free(buffer);
        return NULL;
    }
    buffer[ret] = '\0';

    ret = populate_request(request, buffer);
    if (ret == -1)
    {
        send(request->client_connection, "Invalid request...BYE!\n", 24, 0);
        OK = 1;
    }

    while (OK != 1)
    {
        // call service handler
        request->handler_function(request);
        read(request->client_connection, buffer, BUFF_SIZE);
        if (strncmp(buffer, "ACK", 3) == 0)
        {
            OK = 1;
            request->callback_function(request);
        }
    }

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