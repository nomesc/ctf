#include "service_see_reviews.h"

void *service_see_reviews_cb(struct request *req)
{
    free(req->service);
    if (req->allocated)
        free_request(req);
    return NULL;
}

static int send_reviews(int client, struct service_see_reviews service_see_reviews)
{
    int ret;
    switch (service_see_reviews.language_id)
    {
    case ro:
        pthread_mutex_lock(&mutex_ro);
        break;
    case en:
        pthread_mutex_lock(&mutex_en);
    default:
        pthread_mutex_lock(&mutex_int);
        break;
    }
    ret = open(service_see_reviews.reviews, O_RDONLY);
    if (ret == -1)
    {
        send(client, "ERR NO_REVIEWS", 15, MSG_NOSIGNAL);
        return -1;
    }
    struct stat st;
    fstat(ret, &st);
    char *reviews = calloc(st.st_size + 1, 1);
    read(ret, reviews, st.st_size + 1);
    send(client, reviews, st.st_size + 1, MSG_NOSIGNAL);
    switch (service_see_reviews.language_id)
    {
    case ro:
        pthread_mutex_unlock(&mutex_ro);
        break;
    case en:
        pthread_mutex_unlock(&mutex_en);
    default:
        pthread_mutex_unlock(&mutex_int);
        break;
    }
    return 0;
}

int handle_service_see_reviews(struct request *req)
{
    struct service_see_reviews service_see_reviews = *(struct service_see_reviews *)(req->service);
    int ret;
    service_see_reviews.reviews = req->scratchpad;
    if (service_see_reviews.language_id > language_id_max)
    {
        send(req->client_connection, "ERR", 4, MSG_NOSIGNAL);
        return -1;
    }
    switch (service_see_reviews.language_id)
    {
    case en:
        strncpy(service_see_reviews.reviews, "./data/feedback/feedback_en.txt", 32);
        break;
    case ro:
        strncpy(service_see_reviews.reviews, "./data/feedback/feedback_ro.txt", 32);
        break;
    default:
        /* Never reached */
        break;
    }
    ret = send_reviews(req->client_connection, service_see_reviews);
    return 0;
}
