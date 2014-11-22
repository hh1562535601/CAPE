#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

#define NODE0 "node0"
#define NODE1 "node1"

int node0 (const char *url)
{int i;
  int sock = nn_socket (AF_SP, NN_PULL);
  assert (sock >= 0);
  assert (nn_bind (sock, url) >= 0);
  for(i=0;i<3;i++)
    {
      char *buf = NULL;
      int bytes = nn_recv (sock, &buf, NN_MSG, 0);
      assert (bytes >= 0);
      printf ("NODE0: RECEIVED \"%d\"\n", i);
      nn_freemsg (buf);
    }
}

int node1 (const char *url, const char *msg)
{int i;
  int sz_msg = strlen (msg) + 1; // '\0' too
  int sock = nn_socket (AF_SP, NN_PUSH);
  assert (sock >= 0);
  assert (nn_connect (sock, url) >= 0);
  for(i=0;i<3;i++)
  {printf ("NODE1: SENDING \"%d\"\n", i);
  printf("before send!\n");
  int bytes = nn_send (sock, msg, sz_msg, 0);
  printf("after send!\n");
  assert (bytes == sz_msg);}
  return nn_shutdown (sock, 0);
}

int main (const int argc, const char **argv)
{
  if (strncmp (NODE0, argv[1], strlen (NODE0)) == 0 && argc > 1)
    return node0 (argv[2]);
  else if (strncmp (NODE1, argv[1], strlen (NODE1)) == 0 && argc > 2)
    return node1 (argv[2], argv[3]);
  else
    {
      fprintf (stderr, "Usage: pipeline %s|%s <URL> <ARG> ...'\n",
               NODE0, NODE1);
      return 1;
    }
}

/*compile and output.

gcc pipeline.c /usr/local/lib/libnanomsg.a -o pipeline
./pipeline node0 ipc:///tmp/pipeline.ipc & node0=$! && sleep 1
./pipeline node1 ipc:///tmp/pipeline.ipc "Hello, World!"
./pipeline node1 ipc:///tmp/pipeline.ipc "Goodbye."
kill $node0

NODE1: SENDING "Hello, World!"
NODE0: RECEIVED "Hello, World!"
NODE1: SENDING "Goodbye."
NODE0: RECEIVED "Goodbye."
*/
