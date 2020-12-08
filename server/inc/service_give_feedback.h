#ifndef SERVICE_GIVE_FEEDBACK_H
#define SERVICE_GIVE_FEEDBACK_H

#include "globals.h"
#include "request.h"

struct service_give_feedback
{
    char *name;
    uint32_t feedback_len;
    uint8_t language_id;
    uint8_t solved;
};

struct write_feedback
{
    char name[128];
    char *(*write_and_thank_function)(struct request *, struct write_feedback *);
    uint8_t allocted;
    char *feedback;
};

void *service_give_feedback_cb(struct request *req);

int handle_service_give_feedback(struct request *req);

int feedback_init();

#endif