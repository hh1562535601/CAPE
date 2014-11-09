#include "crawler.h"

void crawler(void *arg)
{
	char **pp;
	struct in_addr addr;
	struct hostent *hostp;
	int sockfd;
	struct sockaddr_in serv_socket;
	int port = 80;
	//char ip[] = "220.181.6.175"; //ip地址，可以通过gethostbyname来获取
	char file_loc[] = "./test.html"; //下载的存放位置
	char *body=(char*)malloc(1024*sizeof(char));
	//char *url=(char*)malloc(512*sizeof(char));
	char url[512]="http://";//add the prefix of http protocol manually.

	hostp = gethostbyname((char*)arg);
	//char *ip = inet_ntoa(((struct in_addr *)(*(hostp->h_addr_list)))->s_addr);
	pp = hostp->h_addr_list; 
	addr.s_addr = ((struct in_addr *)*pp)->s_addr;
	char *ip = inet_ntoa(addr);
	strcat(url,*(hostp->h_aliases));
printf("ip:%s\n",ip);
printf("url:%s\n",*(hostp->h_aliases));

	bzero(&serv_socket, sizeof(struct sockaddr_in));
	serv_socket.sin_family = AF_INET;
	serv_socket.sin_port = htons(port);
	inet_pton(AF_INET, ip, &serv_socket.sin_addr);

	sockfd = socket(AF_INET, SOCK_STREAM, 0); 

	int flag = connect(sockfd, (struct sockaddr *)&serv_socket, sizeof(serv_socket)); //建立和HTTP服务器的TCP链接
	if(flag < 0)
	{
		printf("connect error!!! flag = %d\n", flag);
    	  exit(1);
	}

	send_and_recv(sockfd, "http://www.baidu.com", "GET", "html/text", ip, port, file_loc, body, "Close"); //下载的主体函数

	close(sockfd);
}

void send_and_recv(int sockfd, char * url, char * fun_type, char * accept_type, char * ip, int port, char * file_loc, char * body, char * connection_type) 
{
        char * request = (char *) malloc (4* 1024 * sizeof(char));
        if(body)
                sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %s\r\n\r\n%d", fun_type, url, accept_type, ip, port, connection_type,body,strlen(body));
        else
                sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\n\r\n", fun_type, url, accept_type, ip, port,connection_type);

//以上是在组织请求的头部并打印

        int send = write(sockfd, request, strlen(request));
        //printf("%s", request);
        free(request);
        char * response = (char *) malloc (400*1024 * sizeof(char));
        if(file_loc) {
		    /*FILE *fp;
                if( (fp=fopen(file_loc,"w"))==NULL)
		    {printf("\nfile open error!\n");}*/
                int length;
		   int file = open(file_loc, O_RDWR | O_APPEND | O_CREAT);
                do {
                        length = read(sockfd, response, 400*1024);
                        char * loc = strstr(response, "\r\n\r\n"); //截获返回头部，以\r\n\r\n为标识
                        if(loc) {
                                int loci = loc - response + 4;
                                //write(1, response, loci);//如果是响应头部就打印至屏幕
				write(file, loc+4, length - loci);//如果是响应主体就写入文件
                                //fwrite((void*)(loc+4), sizeof(char), length - loci,fp);//如果是响应主体就写入文件
                        } else {
                                write(file, response, length);
                        }
//printf("\n%s\n",response);
                        if(!length)//注意，因为之前采用的是close方法，也就是说一旦传输数据完毕，则服务器端会断开链接，则read函数会返回0，所以这里 会退出循环。如果采用的是Keep-Alive则服务器不关闭TCP链接，也就说程序将会被阻塞在read函数中，因此要注意的是自己判断是否读到了响应 的结尾，然后在再次调用read之前退出循环。
                                break;
                } while(1);
                close(file);
        } else {
                int length;
                do {
                        length = read(sockfd, response, 1024);
                        //printf("%s", response);
                        if(!length)
                                break;
                } while(1);
        }
        free(response);
}

