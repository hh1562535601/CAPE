#include <assert.h>
#include <string.h>
#include <nanomsg/nn.h>
#include <nanomsg/pair.h>
#include <nanomsg/pipeline.h>
#include <stdio.h>

#define IPC_URL "ipc://./cra_ana.ipc"

int recv_ipc(int sockfd,char *buf,int sz_buf);
int send_ipc(int sockfd,const char *buf);
int send_recv(int sock,const char *url,char *buf);
