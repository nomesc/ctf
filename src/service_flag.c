#include "service_flag.h"

void service_flag_cb(struct request* req)
{
    free((service_flag*)req->service->country_name);
}

int handle_service_flag(struct request *req)
{
    return 1;
}