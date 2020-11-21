#ifndef SERVICE_FLAG_H
#define SERVICE_FLAG_H

#include "request.h"
#include "globals.h"

struct service_flag
{
    uint8_t color;
    uint8_t *country_name;
};

void *service_flag_cb(struct request *req);

int handle_service_flag(struct request *req);

#endif