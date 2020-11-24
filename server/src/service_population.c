#include "service_population.h"
#include "globals.h"

struct country_population
{
    char *country_name;
    int populatie;
};

static struct country_population *population = NULL;
static int population_array_size = 0;
static int service_init = 0;

static void service_population_init()
{
    FILE *f;
    f = fopen("./data/population/population.txt", "r");
    if (f == NULL)
    {
        perror(NULL);
    }
    fscanf(f, "%d", &population_array_size);
    population = malloc(population_array_size * sizeof(struct country_population));
    for (int i = 0; i < population_array_size; i++)
    {
        char * country_name[COUNTTRY_NAME_MX_LEN];

        int country_pop;
        fscanf(f, "%63s", country_name);
        fscanf(f, "%d", &country_pop);
        population[i].country_name = malloc((strlen(country_name) + 1) * sizeof(char));
        strcpy(population[i].country_name, country_name);
        population[i].populatie = country_pop;
    }
    fclose(f);
}

void *service_population_cb(struct request *req)
{
    free(((struct service_population *)(req->service))->country_name);
    return NULL;
}

int handle_service_population(struct request *req)
{
    uint64_t country_population;
    uint64_t i;
    struct service_population *service = (struct service_population *)req->service;
    if (service_init == 0)
    {
        pthread_mutex_lock(&service_population_init_mutex);
        if (service_init == 0)
            service_population_init();
        service_init = 1;
        pthread_mutex_unlock(&service_population_init_mutex);
    }
    if (population_array_size == 0)
    {
        send(req->client_connection, "ERR No info for this country yet1...\n", 37, 0);
        return -1;
    }
    i = 0;
    country_population = (uint64_t)&population;
    while (i < population_array_size)
    {
        if (strncmp(population[i].country_name, service->country_name, 64) == 0)
        {
            if (population[i].populatie <= 0)
                break;
            country_population = population[i].populatie;
            break;
        }
        i++;
    }
    if (i >= population_array_size)
    {
        send(req->client_connection, "ERR No info for this country yet2...\n", 37, 0);
        return -1;
    }
    char *raspuns = malloc(64);
    sprintf(raspuns, "%llu\n", country_population);
    send(req->client_connection, raspuns, strlen(raspuns), 0);
    return 0;
}