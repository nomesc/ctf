#include "service_flag.h"

void *service_flag_cb(struct request *req)
{
    free(((struct service_flag *)(req->service))->country_name);
    free(req->service);
    return NULL;
}

int handle_service_flag(struct request *req)
{
    int client_connection = req->client_connection;
    volatile int c;
    volatile char *volatile flag = NULL;
    char *flag_png = req->scratchpad;
    struct service_flag arg = *(struct service_flag *)(req->service);
    if (req->allocated)
        free_request(req);
    for (int i = 0; i < 256; i++)
    {
        if (arg.country_name[i] == '.' || arg.country_name[i] == '/')
        {
            send(client_connection, "ERR FLAG\n", 10, MSG_NOSIGNAL);
            return -1;
        }
    }
    strncpy(flag_png, "./data/flags/", 14);
    strncat(flag_png, arg.country_name, 256);
    if (arg.color)
    {
        strncat(flag_png, "_color.png", 11);
    }
    else
    {
        strncat(flag_png, "_bw.png", 8);
    }
    FILE *fp = fopen(flag_png, "r");
    if (NULL == fp)
    {
        send(client_connection, "ERR FLAG\n", 10, MSG_NOSIGNAL);
        return -1;
    }
    struct stat st;
    stat(flag_png, &st);
    flag = malloc(st.st_size);
    int i = 0;
    while ((c = getc(fp)) != EOF)
    {
        flag[i] = c;
    }
    send(client_connection, (const void *)flag, st.st_size, MSG_NOSIGNAL);
    fclose(fp);
    return 0;
}