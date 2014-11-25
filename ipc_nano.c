#include "ipc_nano.h"

int send_ipc(int sockfd,const char *buf,char *ipc_url)
{
  int sz_buf = strlen (buf) + 1; // '\0' too
  //int eid;
  if(sz_buf == 1)
  {
  	return 0;
  }
  //int sockfd = nn_socket(AF_SP, NN_PAIR);
  //int timeout=100;
  //assert (sockfd >= 0);
  //assert(nn_setsockopt (sockfd, NN_SOL_SOCKET, NN_SNDTIMEO, &timeout, sizeof (timeout)) >= 0);
  //assert ((eid=nn_connect (sockfd, ipc_url)) >= 0);
  //printf ("NODE1: SENDING \"%s\"\n", buf);
  int bytes;
  //while((bytes = nn_send (sockfd, buf, sz_buf, 0)) == -1);
  do 
  {
  	errno=0;
  	//printf("before nn_send\n");
  	//printf("send buf:%s\n",buf);
  	bytes = nn_send (sockfd, buf, sz_buf, NN_DONTWAIT);
  	if(errno){printf("send error:%s:%s,%d\n",errno==EAGAIN?"EAGAIN":"other errors",nn_strerror(errno),bytes);}
  	//printf("after nn_send\n");
  	sleep(1);
  	/*if(errno == EAGAIN)
  	{
  		break;
  	}
  	if(bytes > 0)
  	{
  		break;
  	}*/
  	//printf("bytes:%d sz_buf:%d\n",bytes);
  }while(errno == EAGAIN);
  /*bytes = nn_send (sockfd, buf, sz_buf, NN_DONTWAIT);
  if(bytes <=0)
  {
  	//printf("%s\n",errno==EAGAIN?"errno:EAGAIN":"other errors");
  	printf("error:%s-%s\n",errno==EAGAIN?"EAGAIN":"other errors",nn_strerror(errno));
  }
  sleep(1);*/
  
  /*switch(errno)
  {
  
case EFAULT:

   { printf("buf is NULL or len is NN_MSG and the message pointer (pointed to by buf) is NULL.");break;}
case EBADF:

    {printf("The provided socket is invalid.");break;}
case ENOTSUP:

    {printf("The operation is not supported by this socket type.");break;}
case EFSM:

    {printf("The operation cannot be performed on this socket at the moment because the socket is not in the appropriate state. This error may occur with socket types that switch between several states.");break;}
case EAGAIN:

    {printf("Non-blocking mode was requested and the message cannot be sent at the moment.");break;}
case EINTR:

    {printf("The operation was interrupted by delivery of a signal before the message was sent.");break;}
case ETIMEDOUT:

    {printf("Individual socket types may define their own specific timeouts. If such timeout is hit, this error will be returned.");break;}
case ETERM:

    {printf("The library is terminating.");break;}

  }*/
  
  //assert (bytes == sz_buf);
  printf("send len:%d\n",bytes);
  //assert (nn_shutdown (sockfd,eid) == 0);
}

int recv_ipc(int sockfd,char *buf,int sz_buf,char *ipc_url)
{
  //int sockfd = nn_socket(AF_SP, NN_PAIR);
  //int timeout=100;
  int eid;
  
  //assert (sockfd >= 0);
  //assert(nn_setsockopt (sockfd, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof (timeout)) >= 0);
  //char *buf = NULL;
  //assert ((eid=nn_bind (sockfd, ipc_url)) >= 0);
 
  //printf("sockfd:%d,buf[0]:%c,sz_buf:%d\n",sockfd,buf[0],sz_buf);
  //printf("ipc_url:%s\n",ipc_url);
  int result;
  //while((result = nn_recv (sockfd, buf, sz_buf, 0)) == -1);
  //errno == EAGAIN || 
  do
  {//printf("recv again\n");
  	//printf("before nn_recv!\n");
  	errno=0;
  	sleep(1);
  	result = nn_recv (sockfd, buf, sz_buf, NN_DONTWAIT);
  	if(errno){printf("recv error:%s-%s\n",errno==EAGAIN?"EAGAIN":"other errors",nn_strerror(errno));}
  	
  	//printf("after nn_recv!\n");
  	/*if(errno == EAGAIN)
  	{
  		break;
  	}
  	if (result > 0)
  	{
  		//printf("recvbuf:%s\n",buf);
  		break;
  	}*/
  }while(errno == EAGAIN && result <= 0);
  
  /*if (result > 0)
    {
      printf ("%s: RECEIVED \"%s\"\n", name, buf);
      nn_freemsg (buf);
    }*/
    //(*buf)[result]='\0';
  printf("error:%s\n",nn_strerror(errno));
  printf("recv len:%d\n",result);
  //assert (nn_shutdown(sockfd,eid) == 0 );
}

/*int send_recv(int sock,const char *url,char *buf)
{
  int to = 100;
  assert (nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);

  recv_ipc(sock,buf);
  sleep(1);
  send_ipc(sock,url);
}*/
