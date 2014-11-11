#include "ipc_nano.h"

int send_ipc(int sock,const char *url)
{
  //printf ("%s: SENDING \"%s\"\n", name, name);
  int sz_n = strlen (url) + 1; // '\0' too
  return nn_send (sock, url, sz_n, 0);
}

int recv_ipc(int sock,char *buf)
{
  //char *buf = NULL;
  int result = nn_recv (sock, &buf, NN_MSG, 0);
  if (result > 0)
    {
      //printf ("%s: RECEIVED \"%s\"\n", name, buf);
      nn_freemsg (buf);
    }
  return result;
}

int send_recv(int sock,const char *url,char *buf)
{
  int to = 100;
  assert (nn_setsockopt (sock, NN_SOL_SOCKET, NN_RCVTIMEO, &to, sizeof (to)) >= 0);

  recv_ipc(sock,buf);
  sleep(1);
  send_ipc(sock,url);
}
