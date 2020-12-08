#include "service_add_population.h"

int handle_add_population(struct request *req)
{
    struct add_population *population_info = (struct add_population *)req->service;

    char *old_file_path = "./data/population/population.txt";
    char *new_file_path = "./data/population/new_population.txt";

    FILE *f;
    FILE *new_f;
    const int LINE_SIZE = 4096;
    char file_line[LINE_SIZE];
    int no_countries = 0;
    int country_found = 0;
    char new_line[LINE_SIZE];

    sprintf(req->scratchpad, "%s", population_info->country_name);

    pthread_mutex_lock(&population_add_lock);

    f = fopen(old_file_path, "r");

    // checking if country already exists
    fgets(file_line, LINE_SIZE, f);
    for (int i = 0; i < strlen(file_line) - 1; i++)
    {
        no_countries = no_countries * 10 + file_line[i] - '0';
    }

    while (fgets(file_line, LINE_SIZE, f) != NULL)
    {
        if (strncmp(file_line, population_info->country_name, strlen(population_info->country_name)) == 0)
        {
            country_found = 1;
            break;
        }
    }

    if (!country_found)
    {
        no_countries += 1;
    }

    // updating the file by creating a new one and deleting the old one
    fseek(f, 0, SEEK_SET);
    new_f = fopen(new_file_path, "w");

    fgets(file_line, LINE_SIZE, f);
    fprintf(new_f, "%d\n", no_countries);
    sprintf(new_line, "%s %llu\n", req->scratchpad, population_info->population);
    while (fgets(file_line, LINE_SIZE, f) != NULL)
    {
        if (strncmp(file_line, population_info->country_name, strlen(population_info->country_name)) == 0 &&
            file_line[strlen(population_info->country_name)] == ' ')
        {
            fputs(new_line, new_f);
        }
        else
        {
            fputs(file_line, new_f);
        }
    }
    if (!country_found)
    {
        fputs(new_line, new_f);
    }

    refresh_population_info();

    fclose(new_f);
    fclose(f);

    remove(old_file_path);
    rename(new_file_path, old_file_path);

    pthread_mutex_unlock(&population_add_lock);

    return 0;
}

void *service_add_population_cb(struct request *req)
{
    free(((struct add_population *)(req->service))->country_name);
    free(req->service);
    if (req->allocated)
        free_request(req);
    return NULL;
}
