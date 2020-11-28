#include "service_flag.h"

void *service_flag_cb(struct request *req)
{
    free(((struct service_flag *)(req->service))->country_name);
    free(req->service);
    return NULL;
}

int handle_service_flag(struct request *req)
{
    int client_connection = req->client_connection;
    struct service_flag arg = *(struct service_flag *)(req->service);
    if(req->allocated)
        free(req);

    

    send(client_connection, "FLAG\n", 6, 0);
    return 1;
}