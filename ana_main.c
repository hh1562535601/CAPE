#include "analyzer.h"

int main()
{
	setbuf(stdout,NULL);

    	urlset us;
    	int i;
    	int sockfd = nn_socket (AF_SP, NN_PAIR);
    	/*ichar *recvbuf=(char*)malloc(256*1024*sizeof(char));
	int timeout=100;
		
	nn_setsockopt (sockfd, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof (timeout));*/

	assert (sockfd >= 0);
	assert (nn_bind (sockfd, "ipc://./cra_ana.ipc") >= 0);

    	us.n_read=-1;
    	us.n_write=-1;
	us.url=(char**)malloc(160*1024*sizeof(char*));

	for(i=0;i<160*1024;i++)
	{
		us.url[i]=NULL;
	}
//printf("us.n_write:%d\n",us.n_write);
	while(1)
	{
		analyze(sockfd,&us);
		
		//printf("infinite loop\n");
	}
	//recv_ipc(recvbuf,256*1024,"ipc://./cra_ipc.ipc");
	//assert(nn_shutdown (sockfd, 0));

	return 0;
}
