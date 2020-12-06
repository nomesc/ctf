#include "service_give_feedback.h"

static char thanks_ro[] = "Multumim, ";
static char thanks_en[] = "Thank you, ";

static char *write_feedback_en(struct write_feedback *write_feedback)
{
    pthread_mutex_lock(&mutex_en);
    int fd = open("./data/feedback/feedback_en.txt", O_WRONLY | O_APPEND);
    if (fd)
    {
        ERROR("Can't open ./data/feedback/feedback_en.txt");
        exit(-1);
    }
    write(fd, "------------------------------\n", 32);
    write(fd, write_feedback->feedback, strlen(write_feedback->feedback));
    pthread_mutex_unlock(&mutex_en);
    char *ret = calloc(strlen(thanks_en + 128), 1);
    strncpy(ret, thanks_en, strlen(thanks_en));
    strncpy(ret + strlen(thanks_en), write_feedback->name, 64);
    if (write_feedback->allocted)
        free(write_feedback);
    return ret;
}

static char *write_feedback_ro(struct write_feedback *write_feedback)
{
    pthread_mutex_lock(&mutex_ro);
    int fd = open("./data/feedback/feedback_ro.txt", O_WRONLY | O_APPEND);
    if (fd)
    {
        ERROR("Can't open ./data/feedback/feedback_ro.txt");
        exit(-1);
    }
    write(fd, "------------------------------\n", 32);
    write(fd, write_feedback->feedback, strlen(write_feedback->feedback));
    pthread_mutex_unlock(&mutex_ro);
    char *ret = calloc(strlen(thanks_en + 128), 1);
    strncpy(ret, thanks_ro, strlen(thanks_en));
    strncpy(ret + strlen(thanks_ro), write_feedback->name, 64);
    if (write_feedback->allocted)
        free(write_feedback);
    return ret;
}

static char *write_feedback_int(struct write_feedback *write_feedback)
{
    pthread_mutex_lock(&mutex_int);
    int fd = open("./data/feedback/feedback_int.txt", O_WRONLY | O_APPEND);
    if (fd)
    {
        ERROR("Can't open ./data/feedback/feedback_int.txt");
        exit(-1);
    }
    write(fd, "------------------------------\n", 32);
    write(fd, write_feedback->feedback, strlen(write_feedback->feedback));
    pthread_mutex_unlock(&mutex_int);
    char *ret = calloc(strlen(thanks_en + 128), 1);
    strncpy(ret, thanks_en, strlen(thanks_en));
    strncpy(ret + strlen(thanks_en), write_feedback->name, 64);
    if (write_feedback->allocted)
        free(write_feedback);
    return ret;
}

int feedback_init()
{
    int ret = 0;
    ret |= pthread_mutex_init(&mutex_en, NULL);
    ret |= pthread_mutex_init(&mutex_ro, NULL);
    ret |= pthread_mutex_init(&mutex_int, NULL);
    return ret;
}

int feed_back_close()
{
    int ret = 0;
    ret |= pthread_mutex_destroy(&mutex_en);
    ret |= pthread_mutex_destroy(&mutex_ro);
    ret |= pthread_mutex_destroy(&mutex_int);
    return 0;
}

void *service_give_feedback_cb(struct request *req)
{
    free(((struct service_give_feedback *)(req->service))->name);
    free(req->service);
    if (req->allocated)
        free_request(req);
    return NULL;
}

int handle_service_give_feedback(struct request *req)
{
    struct write_feedback write_feedback = {0};
    struct service_give_feedback service_give_feedback = *(struct service_give_feedback *)(req->service);
    if (0 != service_give_feedback.solved)
    {
        send(req->client_connection, req->scratchpad, strlen(req->scratchpad), MSG_NOSIGNAL);
        return 0;
    }
    char *feedback = calloc(service_give_feedback.feedback_len, 1);
    if (feedback == NULL)
    {
        strcpy(req->scratchpad, "ERR");
        send(req->client_connection, "ERR", 4, MSG_NOSIGNAL);
        service_give_feedback.solved = 1;
        return -1;
    }
    read(req->client_connection, feedback, service_give_feedback.feedback_len);
    write_feedback.feedback = feedback;
    switch (service_give_feedback.language_id)
    {
    case ro:
        write_feedback.write_and_thank_function = write_feedback_ro;
        break;
    case en:
        write_feedback.write_and_thank_function = write_feedback_en;
        break;
    default:
        write_feedback.write_and_thank_function = write_feedback_int;
        break;
    }
    strcpy(write_feedback.name, service_give_feedback.name);
    char *response = write_feedback.write_and_thank_function(&write_feedback);
    strcpy(req->scratchpad, write_feedback.write_and_thank_function(&write_feedback));
    send(req->client_connection, response, strlen(response), MSG_NOSIGNAL);
    service_give_feedback.solved = 1;
    free(response);
    free(feedback);
    return 0;
}
