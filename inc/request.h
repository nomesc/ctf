//struct, camp pt socket-ul clientului, int type, void * service
//dispatch

struct request
{
    int client_connection;
    int type;
    void * service;
};

void dispatch(struct request *);