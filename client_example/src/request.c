#include "request.h"

const char *server = "127.0.0.1";

int just_connect(struct request *request)
{
    puts("Creating request");
    int ret;

    // Creating socket file descriptor
    request->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (request->fd == -1)
    {
        perror("Could not create socket file descriptor");
        return -1;
    }

    request->server_address.sin_family = AF_INET;
    request->server_address.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, server, &request->server_address.sin_addr);
    if (ret <= 0)
    {
        puts("Could not fill adress");
        return ret;
    }

    ret = connect(request->fd, (struct sockaddr *)&request->server_address, sizeof(request->server_address));
    if (ret < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    puts("Just created");
    return 0;
}

int create_request(struct request *request, uint32_t expected_length)
{
    puts("Creating request");
    int ret;

    request->expected_length = expected_length;
    request->response = malloc(request->expected_length);

    // Creating socket file descriptor
    request->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (request->fd == -1)
    {
        perror("Could not create socket file descriptor");
        return -1;
    }

    request->server_address.sin_family = AF_INET;
    request->server_address.sin_port = htons(PORT);
    ret = inet_pton(AF_INET, server, &request->server_address.sin_addr);
    if (ret <= 0)
    {
        puts("Could not fill adress");
        return ret;
    }

    ret = connect(request->fd, (struct sockaddr *)&request->server_address, sizeof(request->server_address));
    if (ret < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    ret = send(request->fd, request->message, request->message_length, 0);
    if (ret == -1)
    {
        perror("send");
        return ret;
    }

    puts("Created request");
    return 0;
}

int read_response(struct request *request)
{
    puts("Reading response");
    int ret;
    ret = read(request->fd, request->response, request->expected_length);
    if (ret == -1)
    {
        perror("read");
        return ret;
    }
    puts("Read response");
    return 0;
}

int update_request(struct request *request, uint32_t expected_length)
{
    int ret;

    free(request->response);
    request->expected_length = expected_length;
    request->response = calloc(expected_length, 1);

    puts("Sending update...");
    ret = send(request->fd, request->message, request->message_length, 0);
    if (ret == -1)
    {
        perror("send");
        return ret;
    }
    puts("Updated request");
    return 0;
}

int close_request(struct request *request)
{
    int ret;

    free(request->response);

    puts("Closing request");
    ret = send(request->fd, "ACK", 4, 0);
    if (ret == -1)
    {
        perror("send");
        return ret;
    }

    ret = close(request->fd);
    if (ret == -1)
    {
        perror("close");
        return ret;
    }
    puts("Closed request");
    return 0;
}