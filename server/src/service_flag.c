#include "service_flag.h"

void *service_flag_cb(struct request *req)
{
    if (req->allocated)
        free(req);
    free(((struct service_flag *)(req->service))->country_name);
    free(req->service);
    return NULL;
}

int handle_service_flag(struct request *req)
{
    send(req->client_connection, "FLAG\n", 6, 0);
    return 1;
}