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

int analyze(char *bfbuf)
{
    //FILE *fp = NULL;
    size_t len; /*   store   error   message   length   */
    regex_t re; /*   store   compilned   regular   expression   */
    regmatch_t subs[SUBSLEN]; /*   store   matched   string   position   */
    char matched[BUFLEN]; /*   store   matched   strings   */
    char errbuf[EBUFLEN]; /*   store   error   message   */
    int err, i;
    char *curpath=(char*)malloc(1024*sizeof(char));
    //ulong slen=256*1024;
 
    //char string[] = "AAAAabaaababAbAbCdCd123123   11(123){12} ";
    //char pattern[] = "(\\([0-9]+\\))(\\{[0-9]+\\}{1})$ ";
    char *recvbuf=(char*)malloc(100*1024*sizeof(char));
    //char *srcbuf=(char*)malloc(256*1024*sizeof(char));
    char *p=NULL;
    //char pattern[] ="([/w-]+/.)+[/w-]+.([^a-z])(/[/w-: ./?%&=]*)?|[a-zA-Z/-/.][/w-]+.([^a-z])(/[/w-: ./?%&=]*)?";
    //char pattern[] ="([http|https]://)?([/w-]+/.)+[/w-]+(/[/w- ./?%&amp;=]*)?";
    //char pattern[] ="<a href=\"([^\"]+)\"[^>]*>[^<]+</a>";
    char pattern[] ="<a [^>]*href=\"([^jh\"][^\"]*)\"";
    //char pattern[] = "<a href.+?+\"";
    //char pattern[] = "<a[^>]*?\s?href\s*?=\s*?['\"](?!javascript)(.+?)['\"](?:(?:\s*?)+?.*?)*?>";
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
    //recv_ipc(sockfd,recvbuf,1024*1024);  
     
    //printf("after recv!\n");
    //printf("before file open!\n");
    FILE *file1=fopen("./cra_send.dat","a+");
    //printf("after file open!\n");
    fread(recvbuf,sizeof(char),100*1024,file1);
    //fclose(file1);
    //printf("after file write!\n");
    //printf("recvbuf:%s\n",recvbuf);
    /*FILE *file=fopen("./test_ana.html","a");
    if(ungzip(srcbuf,256*1024,file) != 1)
    {
    	printf("uncompress failed!\n");
    	free(recvbuf);
    	free(srcbuf);
    	regfree(&re);
    	return 0;
    }
    free(recvbuf);
    p=srcbuf;
    printf("%s",p);
 
    
    //int file = open("./test_ana.html", O_RDWR | O_APPEND | O_CREAT,S_IRWXU);
   
    //fprintf(file,"%s",p);
    fclose(file);*/
    if(recvbuf[0] == '\0')
    {
    	printf("recv failed!\n");
    	return 0;
    }
    //puts("before extract!");
    if(!extract_curpath(recvbuf,curpath))
    {
    	return 0;
    }
    //puts("after extract!");
    FILE *file=fopen("./curpath.dat","a");
    //fprintf(file,"curpath:%s\n",curpath);
    fclose(file);
    p=recvbuf;  
    
    if(!p)
    {
    	return 0;
    }
    
    while(1)
     {	//printf("while loop\n");
		err = regexec(&re, p, (size_t)SUBSLEN, subs, 0);

		if (err == REG_NOMATCH)
                {
                    printf("Sorry,   no   match   ...\n");
                    regfree(&re);                   
                    //free(recvbuf);
                    //free(curpath);
                    //if(recvbuf){puts("reg_nomatch before nn_freemsg");nn_freemsg(recvbuf);puts("reg_nomatch after nn_freemsg");}
		        return 0;       //return value is boolean,not errno.       
                }
             else if (err)
                {
                      regerror(err, &re, errbuf, sizeof(errbuf));
                      printf("error:   regexec:   %s\n ", errbuf);
			   regfree(&re);
			   //puts("2");if(recvbuf){nn_freemsg(recvbuf);}
			   //free(recvbuf);
			   //free(curpath);
                      return 0;
                }	
		//printf("%d\n",err);
		static int matched_count=0;
		
		matched_count++;
		printf("matched_count:%d\n",matched_count);
		
		printf("match  begin: %d, end: %d, ",subs[1].rm_so, subs[1].rm_eo);
             len = subs[1].rm_eo - subs[1].rm_so;
             memcpy(matched, p + subs[1].rm_so, len);
             matched[len] = '\0';
             //printf("%s\n",matched);
             puts("before analyzerUrl");
             analyseUrl(bfbuf, curpath,matched);
             puts("after analyzerUrl");
             //extract(sockfd,matched,pus);
            
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
 
 
    //if(recvbuf){nn_freemsg(recvbuf);}
    //free(curpath);
    //free(recvbuf);
    regfree(&re);

   return 1;
}

int extract(int sockfd,char *string,urlset *pus)
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
      pus->url[pus->n_write]=(char*)malloc(1024*sizeof(char));
      strncpy(pus->url[pus->n_write],loc1,len); 
      pus->url[pus->n_write][len]='\0';
   
      //send_ipc(sockfd,pus->url[pus->n_write]);
      //printf("send:pus->url[%d]:%s  len:%d\n",pus->n_write,pus->url[pus->n_write],len);
      //printf("send successfully!\n");
      
      return 1;
}

int extract_curpath(char *buf,char *curpath)
{
	if(!buf || !curpath)
	{
		return 0;
	}
	
	char *loc1=strstr(buf,"!@#");
	
	if(loc1 == NULL)
	{
		return 0;
	}
	//puts("before loc2!");
	printf("loc1:%s\n",loc1);
	char *loc2=strrchr(loc1+3,'/');
	
	if(loc2 == NULL)
	{
		return 0;
	}
	
	int len=loc2-loc1-3;
	//printf("loc2:%s\nlen:%d\n",loc2,len);
	strncpy(curpath,loc1+3,len);
	
	curpath[len]='/';
	curpath[len+1]='\0';
	
	printf("strlen(curpath):%d\n",strlen(curpath));
	
	/*loc1=strstr(curpath,"#@!");
	
	if(loc1)
	{
		strncpy(curpath,loc1+3,len-3);
	}*/
	
	return 1;
}

