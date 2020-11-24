#ifndef SERVICE_ADD_POPULATION_H
#define SERVICE_ADD_POPULATION_H
#include "globals.h"
#include "service_population.h" // refresh population info

struct add_population
{
    char * country_name;
    uint64_t population;
};

void *service_add_population_cb(struct request *req);

int handle_add_population(struct request *req);
#endif