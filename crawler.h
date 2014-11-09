#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>
#include <fcntl.h>

#include "threadpool.h"

#define NUM_THREADS 1

void send_and_recv(int sockfd, char * url, char * fun_type, char * accept_type, char * ip, int port, char * file_loc, char * body, char * connection_type);
void crawler(void *arg);
