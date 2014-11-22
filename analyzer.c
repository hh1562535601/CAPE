#include "analyzer.h"

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

/*int ipc_ana(const char *url,char *buf)
{
  int sock = nn_socket (AF_SP, NN_PAIR);
  assert (sock >= 0);
  assert (nn_bind (sock, IPC_URL) >= 0);
  send_recv(sock,url,buf);
  return nn_shutdown (sock, 0);
}*/

int analyze(urlset *pus)
{
    //FILE *fp = NULL;
    size_t len; /*   store   error   message   length   */
    regex_t re; /*   store   compilned   regular   expression   */
    regmatch_t subs[SUBSLEN]; /*   store   matched   string   position   */
    char matched[BUFLEN]; /*   store   matched   strings   */
    char errbuf[EBUFLEN]; /*   store   error   message   */
    int err, i;
 
    //char string[] = "AAAAabaaababAbAbCdCd123123   11(123){12} ";
    //char pattern[] = "(\\([0-9]+\\))(\\{[0-9]+\\}{1})$ ";
    char *recvbuf=(char*)malloc(256*1024*sizeof(char));
    char *p=NULL;
    //char pattern[] ="([/w-]+/.)+[/w-]+.([^a-z])(/[/w-: ./?%&=]*)?|[a-zA-Z/-/.][/w-]+.([^a-z])(/[/w-: ./?%&=]*)?";
    //char pattern[] ="([http|https]://)?([/w-]+/.)+[/w-]+(/[/w- ./?%&amp;=]*)?";
    //char pattern[] ="<a href=\"([^\"]+)\"[^>]*>[^<]+</a>";
    //char pattern[] ="http://[^\"]*";
    //char pattern[] = "<a href.+?+\"";
    char pattern[] = "<a href=\"http://[^\"]*\"";
    //printf("String   :   %s\n ", string);
    //printf("Pattern:   \" %s \" \n", pattern);
 
    /*if(NULL == (fp = fopen(FILE_PATH)))
    {
        printf("Open error\n");
        return 0;
    }*/
    /*   compile   regular   expression   */
    err = regcomp(&re, pattern, REG_EXTENDED);
 
    if (err)
    {
        regerror(err, &re, errbuf, sizeof(errbuf));
        fprintf(stderr, "error:   regcomp:   %s\n ", errbuf);
        regfree(&re);
        return 0;
    }
    //printf("Total   has   subexpression:   %d\n ", re.re_nsub);
 
    /*   execute   pattern   match   */
    // regexec (regex_t *compiled, char *string, size_t nmatch, regmatch_t matchptr [], int eflags)
    //fread(string,1,256*1024,fp);
    //printf("before recv!\n");
    recvbuf[0]='\0';
    recv_ipc(recvbuf,256*1024,"ipc://./cra_ipc.ipc");
    //printf("after recv!\n");
     
    p=recvbuf;
    //printf("%s",p);
    if(p[0]=='\0')
    {
    	return 0;
    }
    
    //int file = open("./test_ana.html", O_RDWR | O_APPEND | O_CREAT,S_IRWXU);
    FILE *file=fopen("./test_ana.html","a");
    fprintf(file,"%s",p);
    close(file);
    
    while(1)
     {	//printf("while loop\n");
		err = regexec(&re, p, (size_t)SUBSLEN, subs, 0);

		if (err == REG_NOMATCH)
                {
                    printf("Sorry,   no   match   ...\n");
                    regfree(&re);
                    //nn_freemsg(recvbuf);
                    free(recvbuf);
		        return 0;               
                }
             else if (err)
                {
                      regerror(err, &re, errbuf, sizeof(errbuf));
                      printf("error:   regexec:   %s\n ", errbuf);
			   regfree(&re);
			   //nn_freemsg(recvbuf);
			   free(recvbuf);
                      return 0;
                }	
		//printf("%d\n",err);
		printf("match  begin: %d, end: %d, ",subs[0].rm_so, subs[0].rm_eo);
             len = subs[0].rm_eo - subs[0].rm_so+1;
             memcpy(matched, p + subs[0].rm_so, len);
             matched[len] = '\0';
             printf("%s\n",matched);
               
             extract(matched,pus);
            
             //send_ipc(matched);
      
		//ipc_ana(matched,buf);

		p+=subs[0].rm_eo;

		if(*p=='\0')
		{
			break;
		}
        }
        
 
       /*if   no   REG_NOMATCH   and   no   error,   then   pattern   matched  
        printf("\nOK,   has   matched   ...\n\n");
	  
        for (i = 0; i<re.re_nsub; i++) 
           {
            printf("subexpression   %d   begin:   %d,   end:   %d,   ", i,subs[i].rm_so, subs[i].rm_eo);
            len = subs[i].rm_eo - subs[i].rm_so;
            memcpy(matched, string + subs[i].rm_so, len);
            matched[len] = '\0';
            printf("match:   %s\n ", matched);
          } */
 
 
    //nn_freemsg(recvbuf);
    free(recvbuf);
    regfree(&re);

   return 1;
}

int extract(char *string,urlset *pus)
{
	int i;
	char *loc1=string;
	
	for(i=0;i<3;i++)
	{
		loc1=strstr(loc1+1,"/");
	}
	//char *loc1=strstr(string,"\"");
	char *loc2=strstr(loc1+1,"\"");
	int len=loc2-loc1-1;
	
	if(len<=0)
	{
		return 0;
	}
	
	pus->n_write++; 
      //printf("pus->n_write:%d\n",pus->n_write);
      pus->url[pus->n_write]=(char*)malloc(128*sizeof(char));
      strncpy(pus->url[pus->n_write],loc1,len); 
      pus->url[pus->n_write][len]='\0';
   
      send_ipc(pus->url[pus->n_write],"ipc://./ana_ipc.ipc");
      //printf("send:pus->url[%d]:%s  len:%d\n",pus->n_write,pus->url[pus->n_write],len);
      //printf("send successfully!\n");
      
      return 1;
}

