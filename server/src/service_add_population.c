#include "service_add_population.h"
/*
// we need a mutex on this, because this function is not thread safe (more exactly, inet_ntoa is not)
static char * get_ip()
{
    char hostbuffer[256];
    struct hostent * host_entry;
    char * IPbuffer = NULL;

    int hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1)
    {
        return NULL;
    }

    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL)
    {
        return NULL;
    }

    char * ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    if (ip == NULL)
    {
        return NULL;
    }


    return IPbuffer = malloc((strlen(ip) + 1) * sizeof(char));
    strcpy(IPbuffer, ip);
}
*/

int handle_add_population(struct request *req)
{
    struct add_population *population_info = (struct add_population *)req->service;

    //TO DO: get ip and check for add_population eligibility

    // TO DO: update paths
    char *old_file_path = "./data/population/population.txt";
    char *new_file_path = "./data/population/new_population.txt";

    FILE *f;
    FILE *new_f;
    const int LINE_SIZE = 256;
    char file_line[LINE_SIZE];
    int no_countries = 0;
    int country_found = 0;

    sprintf(req->scratchpad, "%s %llu\n", population_info->country_name, population_info->population);

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
    while (fgets(file_line, LINE_SIZE, f) != NULL)
    {
        if (strncmp(file_line, population_info->country_name, strlen(population_info->country_name)) == 0 &&
            file_line[strlen(population_info->country_name)] == ' ')
        {
            fputs(req->scratchpad, new_f);
        }
        else
        {
            fputs(file_line, new_f);
        }
    }
    if (!country_found)
    {
        fputs(req->scratchpad, new_f);
    }

    fclose(new_f);
    fclose(f);

    remove(old_file_path);
    rename(new_file_path, old_file_path);

    pthread_mutex_unlock(&population_add_lock);

    return 0;
}

void *service_add_population_cb(struct request *req)
{
    if (req->allocated)
        free(req);
    free(((struct add_population *)(req->service))->country_name);
    free(req->service);
    return NULL;
}
