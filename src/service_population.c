#include "service_population.h"

void *service_population_cb(struct request *req)
{
    free(((struct service_population *)(req->service))->country_name);
    return NULL;
}

int handle_service_population(struct request *req)
{
    return 1;
}