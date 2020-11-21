#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"

//struct, camp pt socket-ul clientului, int type, void * service
//dispatch

struct request
{
    int client_connection;
    int type;
    void * service;
};

void dispatch(struct request *request);

#endif