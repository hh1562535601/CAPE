#include "ipc_nano.h"

int send_ipc(const char *buf)
{
  int sz_buf = strlen (buf) + 1; // '\0' too
  int sock = nn_socket (AF_SP, NN_PAIR);
  int timeout=100;
  assert (sock >= 0);
  assert(nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof (timeout)) >= 0);
  assert (nn_connect (sock, IPC_URL) >= 0);
  //printf ("NODE1: SENDING \"%s\"\n", buf);
  int bytes = nn_send (sock, buf, sz_buf, 0);
  assert (bytes == sz_buf);
  return nn_shutdown (sock, 0);
}

int recv_ipc(char **buf)
{
  int sockfd = nn_socket (AF_SP, NN_PAIR);
  int timeout=100;
  
  assert (sockfd >= 0);
  assert(nn_setsockopt (sockfd, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof (timeout)) >= 0);
  //char *buf = NULL;
  assert (nn_bind (sockfd, IPC_URL) >= 0);
  int result = nn_recv (sockfd, buf, NN_MSG, 0);
  /*if (result > 0)
    {
      printf ("%s: RECEIVED \"%s\"\n", name, buf);
      nn_freemsg (buf);
    }*/
    //(*buf)[result]='\0';
  return nn_shutdown(sockfd,0);
}

/*int send_recv(int sock,const char *url,char *buf)
{
  int to = 100;
  assert (nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);

  recv_ipc(sock,buf);
  sleep(1);
  send_ipc(sock,url);
}*/
