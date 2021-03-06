#include   <stdio.h>
#include   <stdlib.h>
#include   <string.h>
#include   <sys/types.h>
#include   <stdlib.h>
#include   <regex.h>
 
#define   SUBSLEN   100
#define   EBUFLEN   128   /*   error   buffer   length   */
#define   BUFLEN   1024   /*   matched   buffer   length   */
#define      FILE_PATH                "./test.html","r"
 
int main(int argc, char **argv)
{
    FILE *fp = NULL;
    size_t len; /*   store   error   message   length   */
    regex_t re; /*   store   compilned   regular   expression   */
    regmatch_t subs[SUBSLEN]; /*   store   matched   string   position   */
    char matched[BUFLEN]; /*   store   matched   strings   */
    char errbuf[EBUFLEN]; /*   store   error   message   */
    int err, i;
 
    //char string[] = "AAAAabaaababAbAbCdCd123123   11(123){12} ";
    //char pattern[] = "(\\([0-9]+\\))(\\{[0-9]+\\}{1})$ ";
    char string[200000] = {0};
    char *p;
    //char pattern[] ="([/w-]+/.)+[/w-]+.([^a-z])(/[/w-: ./?%&=]*)?|[a-zA-Z/-/.][/w-]+.([^a-z])(/[/w-: ./?%&=]*)?";
    //char pattern[] ="([http|https]://)?([/w-]+/.)+[/w-]+(/[/w- ./?%&amp;=]*)?";
	 //<a href=\"([^\"]+)\"[^>]*>[^<]+</a>  http:[^"]* char pattern[] = "<a href.+?+\"";
    //char pattern[] = "\"http:[^\"]*\"";
	 //char pattern[]="<a href=([^<]*)>([^<]*)</a>";
	 char pattern[]="<a[^\"]*=\"(http://)?((www)?.[^/]*)(/[^/]/htm)?\"";
    //printf("String   :   %s\n ", string);
    printf("Pattern:   \" %s \" \n", pattern);
 
    if(NULL == (fp = fopen(FILE_PATH)))
    {
        printf("Open error\n");
        return 0;
    }
    /*   compile   regular   expression   */
    err = regcomp(&re, pattern, REG_EXTENDED);
 
    if (err)
    {
        len = regerror(err, &re, errbuf, sizeof(errbuf));
        fprintf(stderr, "error:   regcomp:   %s\n ", errbuf);
        exit(1);
    }
    printf("Total   has   subexpression:   %d\n ", re.re_nsub);
 
    /*   execute   pattern   match   */
    // regexec (regex_t *compiled, char *string, size_t nmatch, regmatch_t matchptr [], int eflags)
    fread(string,1,200000,fp);
    p=string;
 
     while(1)
	  {
		err = regexec(&re, p, (size_t)SUBSLEN, subs, 0);

		if (err == REG_NOMATCH)
        {
            fprintf(stderr, "Sorry,   no   match   ...\n");
            regfree(&re);
				exit(1);                
        }
        else if (err)
        {
            len = regerror(err, &re, errbuf, sizeof(errbuf));
            fprintf(stderr, "error:   regexec:   %s\n ", errbuf);
            exit(1);
        }		
		//printf("%d\n",err);
		printf("\nOK,   has   matched   ...\n\n");
		printf("match  begin: %d, end: %d, ",subs[0].rm_so, subs[0].rm_eo);
      len = subs[0].rm_eo - subs[0].rm_so;
      memcpy(matched, p + subs[0].rm_so, len);
      matched[len] = '\0';
      printf("match: %s\n", matched);

		p+=subs[0].rm_eo;

		if(!*p)
		{
			break;
		}
	  }

	   /* if (err == REG_NOMATCH)
        {
            fprintf(stderr, "Sorry,   no   match   ...\n");
            regfree(&re);
		exit(1);                
        }
        else if (err)
        {
            len = regerror(err, &re, errbuf, sizeof(errbuf));
            fprintf(stderr, "error:   regexec:   %s\n ", errbuf);
            exit(1);
        }		
 		
 
       if   no   REG_NOMATCH   and   no   error,   then   pattern   matched  
        printf("\nOK,   has   matched   ...\n\n");
	  
        for (i = 0; i<10; i++) 
           {
            printf("subexpression   %d   begin:   %d,   end:   %d,   ", i,subs[i].rm_so, subs[i].rm_eo);
            len = subs[i].rm_eo - subs[i].rm_so;
            memcpy(matched, string + subs[i].rm_so, len);
            matched[len] = '\0';
            printf("match:   %s\n ", matched);
          } */
 
    regfree(&re);

   return 0;
}
