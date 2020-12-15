#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>
#include <fcntl.h>

#ifdef DEBUG
#define ERROR(x) perror(x)
#else
#define ERROR(x)
#endif

#define PORT 58888
#define CONN_QUEUE_SIZE 128
#define COUNTTRY_NAME_MX_LEN 64

#define ro 0U
#define en 1U
#define language_id_max 2U

#define OFFSETOF(TYPE, ELEMENT) ((uint32_t)(&(((TYPE *)0)->ELEMENT)))

pthread_mutex_t service_population_init_mutex;
pthread_mutex_t population_add_lock;

pthread_mutex_t mutex_en;
pthread_mutex_t mutex_ro;
pthread_mutex_t mutex_int;

#endif