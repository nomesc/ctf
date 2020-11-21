#include "service_flag.h"

void *service_flag_cb(struct request *req)
{
    free(((struct service_flag *)(req->service))->country_name);
    free(req);
    return NULL;
}

int handle_service_flag(struct request *req)
{
    return 1;
}