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
		
	//crawler(url);
	send_ipc("hello","ipc://./cra_ipc.ipc");
	/*while(1)
	{
		recv_ipc(recvbuf,256*1024,"ipc://./ana_ipc.ipc");
		//printf("fasfasf");
		printf("recvbuf:%s\n",recvbuf);
		crawler(recvbuf);
		//nn_freemsg (recvbuf);
	}*/

	
	return 0;
}
