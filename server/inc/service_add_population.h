struct add_population_thr_args
{
    char * country_name;
    int population;
};

void * add_population(void * arg);