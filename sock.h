#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
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

typedef struct sockaddr SA;

int open_clientfd(char *hostname, int port);
int Make_httprequest(int socket);
void send_and_recv(int sockfd, char * url, char * fun_type, char * accept_type, char * ip, int port, char * file_loc, char * body, char * connection_type);
