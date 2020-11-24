#include "globals.h"
#include "request.h"

int main()
{
    int socket_fd = -1;
    int new_con = -1;
    int ret = -1;
    int opt = 1;
    struct sockaddr_in address = {0};
    socklen_t address_len = sizeof(address);
    pthread_t thread = {0};

    // Creating socket file descriptor
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        ERROR("Could not create socket file descriptor");
        return -1;
    }

    ret = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (ret == -1)
    {
        ERROR("Could not set reuse option");
        return -1;
    }

    // Binding socket to address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    ret = bind(socket_fd, (struct sockaddr *)&address, address_len);
    if (ret == -1)
    {
        ERROR("Could not bind to port");
        return -1;
    }

    // Start listening to port
    ret = listen(socket_fd, CONN_QUEUE_SIZE);
    if (ret == -1)
    {
        ERROR("Could not listen on socket");
        return -1;
    }

    pthread_mutex_init(&service_population_init_mutex, NULL);
    pthread_mutex_init(&service_population_init_mutex, NULL);

    while (1)
    {
        new_con = accept(socket_fd, (struct sockaddr *)&address, &address_len);
        if (new_con == -1)
        {
            ERROR("Could not open new connection with the client");
            continue;
        }
        struct request *new_req = malloc(sizeof(struct request));
        new_req->client_connection = new_con;
        new_req->allocated = 1;
        pthread_create(&thread, NULL, dispatch, new_req);
    }
    
    pthread_mutex_destroy(&population_add_lock);
    pthread_mutex_destroy(&population_add_lock);

    return 0;
}