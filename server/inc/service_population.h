#ifndef SERVICE_POPULATION_H
#define SERVICE_POPULATION_H

#include "request.h"
#include "globals.h"

struct service_population
{
    uint8_t *country_name;
};

void *service_population_cb(struct request *req);

int handle_service_population(struct request *req);

#endif