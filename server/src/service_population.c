#include "service_population.h"

struct country_population
{
    char *country_name;
    int populatie;
};

static struct country_population *population = NULL;

// functie statica service_population_init()

void *service_population_cb(struct request *req)
{
    free(((struct service_population *)(req->service))->country_name);
    return NULL;
}

int handle_service_population(struct request *req)
{
    send(req->client_connection, "POPULATION\n", 12, 0);
    return 1;
}