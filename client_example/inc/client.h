struct client
{
    int client_socket;
    int buff_size;
    char * buff;
    char * command;
    char * raspuns_primit;
};


int custom_command(struct client * client, int server_socket);
int acknowledge_command(struct client * client, int server_socket);
char * get_raspuns_primit(struct client * client);
int refresh_raspuns(struct client * client, int server_socket, char * command);