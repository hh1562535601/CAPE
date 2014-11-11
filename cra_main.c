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

	struct event *ev;
	struct event_base *base=event_base_new();
	evutil_socket_t fd;

	struct sockaddr_in serv_socket;

	bzero(&serv_socket, sizeof(struct sockaddr_in));
	serv_socket.sin_family = AF_INET;
	serv_socket.sin_port = htons(80);
	inet_pton(AF_INET, "220.181.6.175", &serv_socket.sin_addr);

	fd = socket(AF_INET, SOCK_STREAM, 0); 

	int flag = connect(fd, (struct sockaddr *)&serv_socket, sizeof(serv_socket)); //建立和HTTP服务器的TCP链接
	if(flag < 0)
	{
		printf("connect error!!! flag = %d\n", flag);
    	  exit(1);
	}

	char * request = (char *) malloc (4* 1024 * sizeof(char));
	sprintf(request, "GET http://www.baidu.com HTTP/1.1\r\nAccept: html/text\r\nHost: 220.181.6.175:80\r\nConnection: Close\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 1234\r\n\r\n4");
	 int send = write(fd, request, strlen(request));
	 free(request);

	ev=event_new(base,fd,EV_READ,cb_func,(char*)"Reading event!");

	event_add(ev,NULL);

	event_base_dispatch(base);

	
	return 0;
}
