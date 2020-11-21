#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"

//struct, camp pt socket-ul clientului, int type, void * service
//dispatch

struct request
{
    void *(*callback_function)(struct request *req);
    void *(*handler_function)(struct request *req);
    int client_connection;
    int type;
    void *service;
};

void *dispatch(void *request);

#endif