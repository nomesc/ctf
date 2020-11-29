#ifndef SERVICE_FLAG_H
#define SERVICE_FLAG_H

#include "request.h"
#include "globals.h"

struct service_flag
{
    char color;
    char country_name[256];
};

void *service_flag_cb(struct request *req);

int handle_service_flag(struct request *req);

#endif