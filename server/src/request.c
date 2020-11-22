#include "request.h"
#include "service_flag.h"
#include "service_population.h"

static int get_arg(char *buf_in, char *arg_out, int argc, int max_characters)
{
    int args_skipped = 0;
    int i = 0;
    // skip first arguments
    while (args_skipped < argc)
    {
        while (i < max_characters && buf_in[i] == ' ')
            i++;
        args_skipped++;
    }
    while (i < max_characters && buf_in[i] != ' ')
    {
        arg_out[i] = buf_in[i];
        i++;
    }
    if (i >= max_characters)
        return -1;
    arg_out[i] = '\0';
    return 0;
}

int populate_request(struct request *req, char *msg, int msg_len)
{
    int i = 0;

    if (strncmp(msg, "FLG", 3) == 0)
    {
        // "POP NUME_TARA 1"
        req->callback_function = service_flag_cb;
        req->handler_function = handle_service_flag;
        req->service = malloc(sizeof(struct service_flag));
        ((struct service_flag *)(req->service))->country_name = malloc(10);
        ((struct service_flag *)(req->service))->color = 1;
        return 0;
    }
    else if (strncmp(msg, "POP", 3) == 0)
    {
        // "POP NUME_TARA"
        req->callback_function = service_population_cb;
        req->handler_function = handle_service_population;
        req->service = malloc(sizeof(struct service_population));
        ((struct service_population *)(req->service))->country_name = malloc(128);
        get_arg(msg, ((struct service_population *)(req->service))->country_name, 1, msg_len);
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
    int actual_len = 0;
    int ret = -1;
    uint32_t OK = 0;
    char *buffer = malloc(BUFF_SIZE);
    struct request *request = (struct request *)arg;

    actual_len = read(request->client_connection, buffer, BUFF_SIZE);
    if (actual_len == -1)
    {
        ERROR("Could not recieve data from client");
        free(buffer);
        return NULL;
    }
    buffer[actual_len] = '\0';

    ret = populate_request(request, buffer, actual_len);
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