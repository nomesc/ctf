#include "request.h"
#include "service_flag.h"
#include "service_population.h"
#include "service_add_population.h"
#include "service_give_feedback.h"
#include "service_see_reviews.h"

#define BUFF_SIZE 4096

void *win(struct request *req)
{
    int ret;
    printf("WIN: %d\n", req->client_connection);
    int fd = open("./data/secrets.txt", O_RDONLY);
    char flag[1024];
    ret = read(fd, flag, 1024);
    flag[ret] = '\0';
    printf("flag:%d %s\n", ret, flag);
    send(req->client_connection, flag, ret, MSG_NOSIGNAL);
    return NULL;
}

struct request *requests = NULL;
uint32_t size;
pthread_mutex_t request_heap;

int init_request_heap(uint32_t elements)
{
    pthread_mutex_init(&request_heap, NULL);
    pthread_mutex_lock(&request_heap);
    requests = calloc(sizeof(struct request), elements);
    if (requests != NULL)
    {
        size = elements;
        pthread_mutex_unlock(&request_heap);
        return 0;
    }
    else
    {
        size = 0;
        pthread_mutex_unlock(&request_heap);
        return -1;
    }
}

struct request *get_request()
{
    pthread_mutex_lock(&request_heap);
    for (int i = 0; i < size; i++)
    {
        if (requests[i].is_used == 0)
        {
            requests[i].is_used = 1;
            pthread_mutex_unlock(&request_heap);
            return &requests[i];
        }
    }
    pthread_mutex_unlock(&request_heap);
    return NULL;
}

void free_request(struct request *req)
{
    pthread_mutex_lock(&request_heap);
    req->is_used = 0;
    pthread_mutex_unlock(&request_heap);
}

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
    if (j == -1)
        return -1;
    return 0;
}

int populate_request(struct request *req, char *msg, int msg_len)
{
    int i = 0;
    int ret = 0;
    if (strncmp(msg, "FLG", 3) == 0)
    {
        // "FLG NUME_TARA 1"
        req->callback_function = service_flag_cb;
        req->handler_function = handle_service_flag;
        req->service = malloc(sizeof(struct service_flag));
        ret |= get_arg(msg, ((struct service_flag *)(req->service))->country_name, 1, 255);
        ret |= get_arg(msg, req->scratchpad, 2, msg_len);
        ((struct service_flag *)(req->service))->color = strtoull(req->scratchpad, NULL, 0);
        return ret;
    }
    else if (strncmp(msg, "POP", 3) == 0)
    {
        // "POP NUME_TARA"
        req->callback_function = service_population_cb;
        req->handler_function = handle_service_population;
        req->service = malloc(sizeof(struct service_population));
        ((struct service_population *)(req->service))->country_name = malloc(msg_len);
        ret |= get_arg(msg, ((struct service_population *)(req->service))->country_name, 1, msg_len);
        return ret;
    }
    else if ((strncmp(msg, "ADD", 3) == 0))
    {
        // "ADD NUME_TARA POPULATIE"
        req->callback_function = service_add_population_cb;
        req->handler_function = handle_add_population;
        req->service = malloc(sizeof(struct add_population));
        ((struct add_population *)(req->service))->country_name = malloc(msg_len);
        ret |= get_arg(msg, ((struct add_population *)(req->service))->country_name, 1, msg_len);
        ret |= get_arg(msg, req->scratchpad, 2, msg_len);
        ((struct add_population *)(req->service))->population = strtoull(req->scratchpad, NULL, 0);
        return ret;
    }
    else if (strncmp(msg, "SEE", 3) == 0)
    {
        // "SEE 1"
        req->callback_function = service_see_reviews_cb;
        req->handler_function = handle_service_see_reviews;
        req->service = malloc(sizeof(struct service_see_reviews));
        char id[128];
        uint32_t size = 128 < msg_len ? 128 : msg_len;
        ret |= get_arg(msg, id, 1, size);
        ((struct service_see_reviews *)(req->service))->language_id = strtoull(id, NULL, 0);
        return ret;
    }
    else if (strncmp(msg, "WRT", 3) == 0)
    {
        // "WRT CLIENT_NAME FEEDBACK_LEN LANG_ID"
        req->callback_function = service_give_feedback_cb;
        req->handler_function = handle_service_give_feedback;
        req->service = malloc(sizeof(struct service_give_feedback));
        ((struct service_give_feedback *)(req->service))->name = malloc(255);
        ret |= get_arg(msg, ((struct service_give_feedback *)(req->service))->name, 1, 136);
        ret |= get_arg(msg, req->scratchpad, 2, msg_len);
        ((struct service_give_feedback *)(req->service))->feedback_len = strtoull(req->scratchpad, NULL, 0);
        ret |= get_arg(msg, req->scratchpad, 2, msg_len);
        ((struct service_give_feedback *)(req->service))->language_id = strtoull(req->scratchpad, NULL, 0);
        return ret;
    }
    else
    {
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
    printf("REQ: %d\n", request->client_connection);
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
        send(request->client_connection, "Invalid Request\n", 17, MSG_NOSIGNAL);
        OK = 1;
    }

    while (OK != 1)
    {
        // call service handler
        request->handler_function(request);
        ret = read(request->client_connection, buffer, BUFF_SIZE);
        if (ret <= 0)
        {
            request->callback_function(request);
            break;
        }
        if (strncmp(buffer, "ACK", 3) == 0)
        {
            OK = 1;
            printf("APELAM:\t%p\n", request->callback_function);
            printf("win:\t%p\n", win);
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