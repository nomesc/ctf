#ifndef REQUEST_H
#define REQUEST_H

#include "globals.h"

struct request
{
    int fd;
    struct sockaddr_in server_address;
    char *response;
    uint32_t expected_length;
    char *message;
    uint32_t message_length;
};

int create_request(struct request *request, uint32_t expected_length);

int read_response(struct request *request);

int update_request(struct request *request, uint32_t expected_length);

int close_request(struct request *request);

#endif