#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"

struct request
{
    void *(*callback_function)(struct request *req);
    uint64_t type;
    uint64_t allocated;
    int (*handler_function)(struct request *req);
    int client_connection;
    void *service;
    uint8_t is_used;
    char scratchpad[2048];
};

void *dispatch(void *request);

int init_request_heap(uint32_t elements);

struct request *get_request();

void free_request(struct request *req);

#endif