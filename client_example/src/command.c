#include "command.h"

int dispatch_command(int server_socket, char * buff, int buff_sz, char * command)
{
    int ret = -1;
    
    ret = send(server_socket, command, strlen(command), 0);
    if (ret == -1) return ret;

    ret = read(server_socket, buff, buff_sz);
    if (ret == -1) return ret;

    // 0 means success
    return 0;
}