#ifdef DEBUG
#define ERROR(x) perror(x)
#else
#define ERROR(x)
#endif

#define PORT 56647
#define CONN_QUEUE_SIZE 128