#include "sock.h"

int main(int argc, char **argv)
{ /* argv[1] is a domain name */
char **pp;
/* or dotted decimal IP addr */
struct in_addr addr;
struct hostent *hostp;
int sockfd;
struct sockaddr_in serv_socket;
int port = 80;
char ip[] = "220.181.6.175"; //ip地址，可以通过gethostbyname来获取
char file_loc[] = "/home/HH/cape/http/temp.html"; //下载的存放位置
char *body=(char*)malloc(400*1024*sizeof(char));

bzero(&serv_socket, sizeof(struct sockaddr_in));
serv_socket.sin_family = AF_INET;
serv_socket.sin_port = htons(port);
inet_pton(AF_INET, ip, &serv_socket.sin_addr);

sockfd = socket(AF_INET, SOCK_STREAM, 0); 

/*if (inet_aton(argv[1], &addr) != 0)
hostp = gethostbyaddr((const char *)&addr, sizeof(addr),AF_INET);
else
hostp = gethostbyname(argv[1]);
printf("official hostname: %s\n", hostp->h_name);
for (pp = hostp->h_aliases; *pp != NULL; pp++)
printf("alias: %s\n", *pp);
for (pp = hostp->h_addr_list; *pp != NULL; pp++) {
addr.s_addr = ((struct in_addr *)*pp)->s_addr;
printf("address: %s\n", inet_ntoa(addr));
}*/

int flag = connect(sockfd, (struct sockaddr *)&serv_socket, sizeof(serv_socket)); //建立和HTTP服务器的TCP链接
if(flag < 0)
{
	printf("connect error!!! flag = %d\n", flag);
      exit(1);
}

send_and_recv(sockfd, "http://www.baidu.com/", "GET", "html/text", ip, port, file_loc, body, "Close"); //下载的主体函数

close(sockfd);

return 0;
}
