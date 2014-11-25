#include "crawler.h"


int main()
{
	setbuf(stdout,NULL);
	/*int i;
	threadpool tp=create_threadpool(NUM_THREADS);
	char *url="www.baidu.com";
	
	for(i=0;i<NUM_THREADS;i++)
	{
		dispatch(tp,crawler,url);
	}

	do_work(tp);

	destroy_threadpool(tp);*/

	char *url="/techqq/index.htm";
	char *recvbuf=(char*)malloc(256*1024*sizeof(char));
	//char *recvbuf=NULL;
	int sockfd=nn_socket(AF_SP,NN_PAIR);//nanomsg socket declaration before tcp socket may cause problems.
	
	assert (sockfd >= 0);
	assert (nn_connect (sockfd, "ipc://./cra_ana.ipc") >= 0);
		
	crawler(sockfd,url);
	//send_ipc("hello","ipc://./cra_ipc.ipc");
	while(1)
	{
		recv_ipc(sockfd,recvbuf,256*1024);
		//printf("fasfasf");
		printf("recvbuf:%s\n",recvbuf);
		while(crawler(sockfd,recvbuf) == 0)
		{
			recv_ipc(sockfd,recvbuf,256*1024);
			printf("recvbuf:%s\n",recvbuf);
			crawler(sockfd,recvbuf);
		}
		
		//nn_freemsg (recvbuf);
	}

	
	return 0;
}
