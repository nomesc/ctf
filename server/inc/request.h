#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"

//struct, camp pt socket-ul clientului, int type, void * service
//dispatch

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