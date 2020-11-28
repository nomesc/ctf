#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"
#include "service_flag.h"
#include "service_population.h"
#include "service_add_population.h"
#include "service_feedback.h"
#include "globals.h"

struct request
{
    void *(*callback_function)(struct request *req);
    uint64_t type;
    uint8_t allocated;
    int (*handler_function)(struct request *req);
    int client_connection;
    void *service;
    char scratchpad[2048];
};

void *dispatch(void *request);

#endif