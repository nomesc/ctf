#include "service_population.h"
#include "globals.h"

struct country_population
{
    char *country_name;
    int populatie;
};

static struct country_population *population = NULL;
static int population_array_size = 0;

static void service_population_init()
{
    FILE * f;
    f = fopen("../data/popultion/popolation.txt", "r");
    fscanf(f, "%d", &population_array_size);
    population = malloc(population_array_size * sizeof(struct country_population));
    for (int i = 0; i < population_array_size; i++)
    {
        char * country_name[COUNTTRY_NAME_MX_LEN];
        int country_pop;
        fscanf(f, '%s', country_name);
        fscanf(f, "%d", &country_pop);
        population[i].country_name = malloc((strlen(country_name) + 1) * sizeof(char));
        population[i].country_name = country_name;
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
    send(req->client_connection, "POPULATION\n", 12, 0);
    return 1;
}