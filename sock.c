#include "sock.h"

int open_clientfd(char *hostname, int port)
{
int clientfd;
struct hostent *hp;
struct sockaddr_in serveraddr;
if ((clientfd =socket(AF_INET, SOCK_STREAM, 0)) < 0)
{return -1; /* check errno for cause of error */}
/* Fill in the server's IP address and port */
if ((hp = gethostbyname(hostname)) == NULL)
{return -2; /* check h_errno for cause of error */}
bzero((char *) &serveraddr, sizeof(serveraddr));
serveraddr.sin_family = AF_INET;
bcopy((char *)hp->h_addr_list[0],
(char *)&serveraddr.sin_addr.s_addr, hp->h_length);
serveraddr.sin_port = htons(port);
/* Establish a connection with the server */
if (connect(clientfd, (SA*)&serveraddr,sizeof(serveraddr)) < 0)
{return -1;}
return clientfd;
}

/*int Make_httprequest(int socket)
{
char req[1024];
sprintf(req, "POST %s HTTP/1.0\r\n", path);
sprintf(req, "Host: %s\r\n", host);
sprintf(req, "User-Agent: " USER_AGENT "\r\n");
sprintf(req, "Content-Type: application/x-www-form-urlencoded\r\n");
sprintf(req, "Content-Length: %d\r\n", strlen(postdata));
sprintf(req, "\r\n"); sprintf(req, "%s", postdata);
Send(socket, req);
}*/

void send_and_recv(int sockfd, char * url, char * fun_type, char * accept_type, char * ip, int port, char * file_loc, char * body, char * connection_type) 
{
        char * request = (char *) malloc (4* 1024 * sizeof(char));
        if(body)
                sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s", fun_type, url, accept_type, ip, port, connection_type, body, strlen(body));
        else
                sprintf(request, "%s %s HTTP/1.1\r\nAccept: %s\r\nHost: %s:%d\r\nConnection: %s\r\n\r\n", fun_type, url, accept_type, ip, port,connection_type);

//以上是在组织请求的头部并打印

        int send = write(sockfd, request, strlen(request));
        printf("%s", request);
        free(request);
        char * response = (char *) malloc (400*1024 * sizeof(char));
        if(file_loc) {
		    /*FILE *fp;
                if( (fp=fopen(file_loc,"w"))==NULL)
		    {printf("\nfile open error!\n");}*/
                int length;
		   int file = open(file_loc, O_RDWR | O_APPEND);
                do {
                        length = read(sockfd, response, 400*1024);
                        char * loc = strstr(response, "\r\n\r\n"); //截获返回头部，以\r\n\r\n为标识
                        if(loc) {
                                int loci = loc - response + 4;
                                write(1, response, loci);//如果是响应头部就打印至屏幕
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
                        printf("%s", response);
                        if(!length)
                                break;
                } while(1);
        }
        free(response);
}


