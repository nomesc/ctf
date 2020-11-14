#ifdef DEBUG
#define ERROR(x) perror(x)
#else
#define ERROR(x)
#endif

#define PORT 56647
#define CONN_QUEUE_SIZE 80000

char g_buffer[1024];