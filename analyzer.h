#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include   <sys/types.h>
#include   <stdlib.h>
#include   <regex.h>
#include   <zlib.h>
#include   <fcntl.h>
//#include   <error.h>

#include   "ipc_nano.h"
 
#define   SUBSLEN   10
#define   EBUFLEN   128  /*   error   buffer   length   */
#define   BUFLEN   1024   /*   matched   buffer   length   */
#define   FILE_PATH     "./test.html","r"
#define BIT_ARRAY_LENGTH 512*1024*sizeof(char)

typedef struct urlset
{
	int n_read;
	int n_write;
	char **url;
}urlset;

int ipc_ana(const char *url,char *buf);
int analyze(char *bfbuf);
int extract(int sockfd,char *string,urlset *pus);

/*code reuse*/
void trimSpace(char *str);
int  getAbsPath(char *url,const char *cururl,char *ans);
int  analyseUrl(char *bfbuf, char *urlcur, char *urlin);
