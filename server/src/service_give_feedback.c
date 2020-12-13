#include "service_give_feedback.h"

static char thanks_ro[] = "Multumim, ";
static char thanks_en[] = "Thank you, ";

static char *write_feedback_en(struct request *req, struct write_feedback *write_feedback)
{
    pthread_mutex_lock(&mutex_en);
    int fd = open("./data/feedback/feedback_en.txt", O_WRONLY | O_APPEND);
    if (fd == -1)
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
    //if (write_feedback->allocted)
    //    myfree(write_feedback);
    return ret;
}

static char *write_feedback_ro(struct request *req, struct write_feedback *write_feedback)
{
    pthread_mutex_lock(&mutex_ro);
    int fd = open("./data/feedback/feedback_ro.txt", O_WRONLY | O_APPEND);
    if (fd == -1)
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
    //if (write_feedback->allocted)
    //    myfree(write_feedback);
    return ret;
}

static char *write_feedback_int(struct request *req, struct write_feedback *write_feedback)
{
    puts("->INT");
    pthread_mutex_lock(&mutex_int);
    int fd = open("./data/feedback/feedback_int.txt", O_WRONLY | O_APPEND);
    if (fd == -1)
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
    //if (write_feedback->allocted)
    //    myfree(write_feedback);
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
    puts("free name");
    myfree(((struct service_give_feedback *)(req->service))->name);
    myfree(req->service);
    if (req->allocated)
        free_request(req);
    return NULL;
}

int handle_service_give_feedback(struct request *req)
{
    int ret;
    char *response = NULL;
    struct write_feedback write_feedback = {0};
    struct service_give_feedback service_give_feedback = *(struct service_give_feedback *)(req->service);
    puts("WRT");
    if (0 != service_give_feedback.solved)
    {
        puts("Already done");
        send(req->client_connection, req->scratchpad, strlen(req->scratchpad), MSG_NOSIGNAL);
        return 0;
    }
    char *feedback = NULL;
    if (service_give_feedback.feedback_len < __offsetof(struct request, is_used))
    {
        puts("GET REQ");
        feedback = (char *)get_request();
    }
    else
    {
        printf("calloc?");
        feedback = calloc(service_give_feedback.feedback_len, 1);
    }
    if (feedback == NULL)
    {
        strcpy(req->scratchpad, "ERR");
        send(req->client_connection, "ERR", 4, MSG_NOSIGNAL);
        service_give_feedback.solved = 1;
        ret = -1;
        puts("feedback null 125");
        goto exit;
    }
    printf("reading %d\n", service_give_feedback.feedback_len - 1);
    ret = read(req->client_connection, feedback, service_give_feedback.feedback_len - 1);
    if (ret <= 0)
    {
        strcpy(req->scratchpad, "ERR");
        service_give_feedback.solved = 1;
        send(req->client_connection, req->scratchpad, strlen(req->scratchpad), MSG_NOSIGNAL);
        ret = -1;
        puts("read 135");
        goto exit;
    }
    printf("read: %p\n", *(uint64_t *)feedback);
    feedback[service_give_feedback.feedback_len] = '\0';
    write_feedback.feedback = feedback;
    switch (service_give_feedback.language_id)
    {
    case ro:
        puts("ro");
        write_feedback.write_and_thank_function = write_feedback_ro;
        break;
    case en:
        puts("en");
        write_feedback.write_and_thank_function = write_feedback_en;
        break;
    default:
        puts("int");
        write_feedback.write_and_thank_function = write_feedback_int;
        break;
    }
    memcpy(write_feedback.name, service_give_feedback.name, strlen(service_give_feedback.name));
    write_feedback.name[strlen(service_give_feedback.name)] = '\0';
    response = write_feedback.write_and_thank_function(req, &write_feedback);
    if (response == NULL)
    {
        strcpy(req->scratchpad, "ERR");
        service_give_feedback.solved = 1;
        send(req->client_connection, req->scratchpad, strlen(req->scratchpad), MSG_NOSIGNAL);
        ret = -1;
        puts("write_feed 158");
        goto exit;
    }
    strcpy(req->scratchpad, response);
    send(req->client_connection, response, strlen(response), MSG_NOSIGNAL);
    service_give_feedback.solved = 1;
exit:
    puts("WRT exit");
    myfree(response);
    if (service_give_feedback.feedback_len < __offsetof(struct request, is_used))
    {
        puts("free REQ");
        free_request((struct request *)feedback);
    }
    else
    {
        puts("free normal");
        myfree(feedback);
    }
    return 0;
}
