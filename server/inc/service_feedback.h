#ifndef SERVICE_FEEDBACK_H
#define SERVICE_FEEDBACK_H

#include "globals.h"
#include "request.h"

#define ro 0U
#define en 1U

static pthread_mutex_t mutex_en;
static pthread_mutex_t mutex_ro;
static pthread_mutex_t mutex_int;

struct service_feedback
{
    char *name;
    uint32_t feedback_len;
    uint8_t language_id;
    uint8_t solved;
};

struct write_feedback
{
    char name[128];
    char *(*write_and_thank_function)(struct write_feedback *);
    uint8_t allocted;
    char *feedback;
};

void *service_feedback_cb(struct request *req);

int handle_service_feedback(struct request *req);

int feedback_init();

#endif