#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include   <sys/types.h>
#include   <stdlib.h>
#include   <regex.h>

#include <fcntl.h>

#include "ipc_nano.h"
 
#define   SUBSLEN   10
#define   EBUFLEN   128  /*   error   buffer   length   */
#define   BUFLEN   1024   /*   matched   buffer   length   */
#define   FILE_PATH     "./test.html","r"

typedef struct urlset
{
	int n_read;
	int n_write;
	char **url;
}urlset;

int ipc_ana(const char *url,char *buf);
int analyze(urlset *pus,int sockfd);

