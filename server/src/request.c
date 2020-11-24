#include "request.h"
#include "service_flag.h"
#include "service_population.h"
#include "service_add_population.h"
#include "globals.h"

#define BUFF_SIZE 4096

static int get_arg(char *buf_in, char *arg_out, int argc, int size)
{
    int args_skipped = 0;
    int i = 0;
    int j = 0;
    // skip first arguments
    while (args_skipped < argc)
    {
        while (i < size && buf_in[i] != ' ' && buf_in[i] != '\n')
        {
            i++;
        }
        args_skipped++;
        while (i < size && buf_in[i] == ' ')
        {
            i++;
        }
    }
    while (i < size && buf_in[i] != ' ' && buf_in[i] != '\n')
    {
        arg_out[j] = buf_in[i];
        i++;
        j++;
    }
    arg_out[j] = '\0';
    return 0;
}

int populate_request(struct request *req, char *msg, int msg_len)
{
    int i = 0;
    puts("->populate_request");
    if (strncmp(msg, "FLG", 3) == 0)
    {
        // "FLG NUME_TARA 1"
        req->callback_function = service_flag_cb;
        req->handler_function = handle_service_flag;
        req->service = malloc(sizeof(struct service_flag));
        ((struct service_flag *)(req->service))->country_name = malloc(msg_len);
        ((struct service_flag *)(req->service))->color = 1;
        return 0;
    }
    else if (strncmp(msg, "POP", 3) == 0)
    {
        // "POP NUME_TARA"
        req->callback_function = service_population_cb;
        req->handler_function = handle_service_population;
        req->service = malloc(sizeof(struct service_population));
        ((struct service_population *)(req->service))->country_name = malloc(msg_len);
        get_arg(msg, ((struct service_population *)(req->service))->country_name, 1, msg_len);
        return 0;
    }
    else if ((strncmp(msg, "ADD", 3) == 0))
    {
        // "ADD NUME_TARA POPULATIE"
        req->callback_function = service_add_population_cb;
        req->handler_function = handle_add_population;
        req->service = malloc(sizeof(struct add_population));
        ((struct add_population *)(req->service))->country_name = malloc(msg_len);
        get_arg(msg, ((struct add_population *)(req->service))->country_name, 1, msg_len);
        get_arg(msg, req->scratchpad, 2, msg_len);
        ((struct add_population *)(req->service))->population = strtoull(req->scratchpad, NULL, 0);
        return 0;
    }
    else
    {
        puts("cacat");
        return -1;
    }
}

void *dispatch(void *arg)
{
    int ret = -1;
    int actual_len = -1;
    uint32_t OK = 0;
    char *buffer = malloc(BUFF_SIZE);
    struct request *request = (struct request *)arg;

    actual_len = read(request->client_connection, buffer, BUFF_SIZE);
    if (actual_len == -1 || actual_len < 3)
    {
        ERROR("Could not recieve data from client");
        free(buffer);
        return NULL;
    }
    buffer[actual_len] = '\0';

    ret = populate_request(request, buffer, actual_len);
    if (ret == -1)
    {
        send(request->client_connection, "Invalid Request\n", 24, 0);
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