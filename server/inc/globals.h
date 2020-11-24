#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#ifdef DEBUG
#define ERROR(x) perror(x)
#else
#define ERROR(x)
#endif

#define PORT 56647
#define CONN_QUEUE_SIZE 128
#define COUNTTRY_NAME_MX_LEN 64

pthread_mutex_t service_population_init_mutex;
pthread_mutex_t population_add_lock;

#endif