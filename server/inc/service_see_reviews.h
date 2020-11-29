#ifndef SERVICE_SEE_REVIEWS_H
#define SERVICE_SEE_REVIEWS_H

#include "globals.h"
#include "request.h"

#define ro 0U
#define en 1U

struct service_see_reviews
{
    uint8_t language_id;
    char reviews[256];
};

void *service_see_reviews_cb(struct request *req);

int handle_service_see_reviews(struct request *req);

#endif