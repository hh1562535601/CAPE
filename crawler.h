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
#include <event2/event.h>

#include "threadpool.h"
#include "ipc_nano.h"

#define NUM_THREADS 1

typedef struct args
{
	int success;
	int sockfd;
	char *url;
}args;

void send_and_recv(int sockfd, char * url, char * fun_type, char * accept_type, char * ip, int port, char * file_loc, char * body, char * connection_type);
int crawler(char *url);
void cb_func(evutil_socket_t fd, short what,void *arg);
