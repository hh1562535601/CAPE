#include "analyzer.h"
#include "ipc_nano.h"

/*int ipc_ana()
{
  int sock = nn_socket (AF_SP, NN_PULL);
  assert (sock >= 0);
  assert (nn_bind (sock, IPC_URL) >= 0);
  while (1)
    {
      char *buf = NULL;
      int bytes = nn_recv (sock, &buf, NN_MSG, 0);
      assert (bytes >= 0);
      //printf ("NODE0: RECEIVED \"%s\"\n", buf);
      nn_freemsg (buf);
    }
}*/

int ipc_ana(const char *url,char *buf)
{
  int sock = nn_socket (AF_SP, NN_PAIR);
  assert (sock >= 0);
  assert (nn_bind (sock, IPC_URL) >= 0);
  send_recv(sock, url,buf);
  return nn_shutdown (sock, 0);
}
