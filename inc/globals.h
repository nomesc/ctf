#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#ifdef DEBUG
#define ERROR(x) perror(x)
#else
#define ERROR(x)
#endif

#define PORT 56647
#define CONN_QUEUE_SIZE 128

#endif