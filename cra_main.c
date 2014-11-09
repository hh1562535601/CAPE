#include "crawler.h"

int main()
{
	int i;
	threadpool tp=create_threadpool(NUM_THREADS);
	char *url="www.baidu.com";

	setbuf(STDOUT,NULL);
	
	for(i=0;i<NUM_THREADS;i++)
	{
		dispatch(tp,crawler,url);
	}

	do_work(tp);

	destroy_threadpool(tp);

	return 0;
}
