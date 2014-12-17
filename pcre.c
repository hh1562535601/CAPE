#include <stdio.h>
#include <string.h>
#include <pcre.h>
#include <ctype.h>

#define MY_DEBUG

//int total_url=0;

int  trie_insert(const char *url);
int trie_lookup(const char *url);

 struct msgBody{
	 ssize_t len;
	 char *content;
	 int  isGzip;
	 char curDir[1024];	 
};


void trimSpace(char *str)
{
	int strstart;
	char *strend;
	if(!str || !str[0])
		return;
	puts("trimSpace");
	strstart=strspn(str,"\t\r\n ");
	memmove(str,str+strstart,strlen(str+strstart)+1);
	strend=str+strlen(str)-1;
	while(isspace(*strend))
		strend-=1;
	*(strend+1)='\0';
}
//     /techqq/ source/      ./
int  getAbsPath(char *url,const char *cururl,char *ans)
{
	char *cur_file=url;//current position
	char  curtmp[1024];//current path
	printf("strlen(cururl):%d\n",strlen(cururl));
	//printf("cururl:%s\n",cururl);
	if(!url || !cururl ||! ans)
		return -1;
	memmove(curtmp,cururl,strlen(cururl)+1);
	puts("getAbsPath");
	if(url[0]=='/')
	{
		puts("case1");
		strcpy(ans,"http://10.108.106.179");
		strcat(ans,url);
		return 1;
	}

	if((cur_file=strstr(url,"../")))
	{
		puts("case2");
		if(cur_file!=url)
		{
			#ifdef MY_DEBUG
			//fprintf(stderr, "Not a standard url:%s\n", url);
			#endif
			return -1;
		}
		else{
		//	puts(curtmp);
			*(strrchr(strrchr(curtmp,'/')-1,'/')+1)='\0';
			cur_file+=3;
			while(strstr(cur_file,"../"))//count the number of ../
			{
				*(strrchr(strrchr(curtmp,'/')-1,'/')+1)='\0';	
				cur_file+=3;
			}
			strcpy(ans,curtmp);
			strcat(ans,cur_file);
			return 1;
		}
	}

	if ( (cur_file=strstr(url,"./")))
	{
		puts("case3");	
		if(cur_file!=url)
		{
			#ifdef MY_DEBUG
			fprintf(stderr, "Not a standard url:%s\n", url);
			#endif
			return -1;
		}
		
		strcpy(ans,cururl);
		strcat(ans,url+2);
		return 1;
	}

	if(url[0]!='.')
	{
	//	puts("case4");
		strcpy(ans,cururl);
		strcat(ans,url);
		return 1;
	}
	#ifdef MY_DEBUG
	fprintf(stderr, "Not a standard url:%s\n", url);
	#endif
	return -1;
}

//decide if I want this url

int  analyseUrl(char *bfbuf, char *urlcur, char *urlin)
{
	static int total_url=0;
	char *urlout=(char*)malloc(1024*sizeof(char));
	FILE *result=fopen("result.dat","a");
	
	trimSpace(urlin);
	puts("analyseUrl");
	puts("11111111");
	
	if(!bfbuf || !urlcur || !urlin)
		{puts("789");return -1;}
		
	if(strstr(urlin,"http://"))//deal with the absolutly address
	{puts("123");
		if(bf_search(bfbuf,urlin)||strstr(urlin,".htm#")|| strstr(urlin,".html#"))
			return -1;
    		
		if(strstr(urlin,"10.108.106.179"))
		{
			return 1;
		}else
			return -1;
	}
	else if(getAbsPath(urlin,urlcur,urlout)<0 || bf_search(bfbuf,urlout)  ||strstr(urlin,".htm#")|| strstr(urlin,".html#"))//delal with the relative addr
	{puts("456");
		
		return -1;	
	}
	
	if(strstr(urlout,"#@!") == urlout)
	{puts("11 12 13");
		strncpy(urlout,urlout+3,strlen(urlout)-3);
	}
	
	puts("end of analyzerUrl");
	total_url+=1;
	fprintf(stderr,"total_url:%d\n",total_url);
	FILE *file=fopen("./ana_send.dat","a+");
    	fprintf(file,"%s\n",urlout);
    	fclose(file);	
	bf_add(bfbuf,urlout);
	//send_ipc(sockfd,urlout);
	
	//fprintf(result, "urlin:%s\nurlcur:%s\nurlout:%s\n",urlin,urlcur,urlout);
	free(urlout);
	fclose(result);
	
	return 1;

}

#if 0
#define OVECCOUNT 510    /* should be a multiple of 3 
#define MAX_BUFFERSIZE 1024000

void
analysePage(struct msgBody*msg,char *urls,FILE*result)
{
	pcre *re;
	const char *error;
	char *pattern="<a[^>]*?\\s?href\\s*?=\\s*?['\"](?!javascript)(.+?)['\"](?:(?:\\s*?)+?.*?)*?>";
	unsigned char *name_table;
	unsigned int option_bits;
	int erroffset;
	int crlf_is_newline;
	int namecount;
	int name_entry_size;
	int ovector[OVECCOUNT]={0};
	int rc, i;
	int utf8;
	char *subject=msg->content;
	int subject_length=strlen(subject);
	strcpy(urls,"\r\n");
	puts("analysePage");
/*************************************************************************
* Now we are going to compile the regular expression pattern, and handle *
* and errors that are detected.                                          *
*************************************************************************/

re = pcre_compile(
  pattern,              /* the pattern */
  0,                    /* default options */
  &error,               /* for error message */
  &erroffset,           /* for error offset */
  NULL);                /* use default character tables */

/* Compilation failed: print the error message and exit */

if (re == NULL)
  {
  	#ifdef MY_DEBUG
	  fprintf(stderr,"PCRE compilation failed at offset %d: %s\n", erroffset, error);
	  #endif
	  return ;
  }

/* Before running the loop, check for UTF-8 and whether CRLF is a valid newline
sequence. First, find the options with which the regex was compiled; extract
the UTF-8 state, and mask off all but the newline options. */

(void)pcre_fullinfo(re, NULL, PCRE_INFO_OPTIONS, &option_bits);
utf8 = option_bits & PCRE_UTF8;
option_bits &= PCRE_NEWLINE_CR|PCRE_NEWLINE_LF|PCRE_NEWLINE_CRLF|
	       PCRE_NEWLINE_ANY|PCRE_NEWLINE_ANYCRLF;

/* If no newline options were set, find the default newline convention from the
build configuration. */

if (option_bits == 0)
  {
  int d;
  (void)pcre_config(PCRE_CONFIG_NEWLINE, &d);
  /* Note that these values are always the ASCII ones, even in
  EBCDIC environments. CR = 13, NL = 10. */
  option_bits = (d == 13)? PCRE_NEWLINE_CR :
	  (d == 10)? PCRE_NEWLINE_LF :
	  (d == (13<<8 | 10))? PCRE_NEWLINE_CRLF :
	  (d == -2)? PCRE_NEWLINE_ANYCRLF :
	  (d == -1)? PCRE_NEWLINE_ANY : 0;
  }

/* See if CRLF is a valid newline sequence. */

crlf_is_newline =
     option_bits == PCRE_NEWLINE_ANY ||
     option_bits == PCRE_NEWLINE_CRLF ||
     option_bits == PCRE_NEWLINE_ANYCRLF;

/* Loop for second and subsequent matches */
for (;;)
  {
  int options = 0;                 /* Normally no options */
  int start_offset = ovector[1];   /* Start at end of previous match */

  /* If the previous match was for an empty string, we are finished if we are
  at the end of the subject. Otherwise, arrange to run another match at the
  same point to see if a non-empty match can be found. */

  if (ovector[0] == ovector[1])
    {
	    if (ovector[0] == subject_length) break;
	    options = PCRE_NOTEMPTY_ATSTART | PCRE_ANCHORED;
    }

  /* Run the next matching operation */

  rc = pcre_exec(
    re,                   /* the compiled pattern */
    NULL,                 /* no extra data - we didn't study the pattern */
    subject,              /* the subject string */
    subject_length,       /* the length of the subject */
    start_offset,         /* starting offset in the subject */
    options,              /* options */
    ovector,              /* output vector for substring information */
    OVECCOUNT);           /* number of elements in the output vector */

  if (rc == PCRE_ERROR_NOMATCH)
    {
    if (options == 0) break;                    /* All matches found */
    ovector[1] = start_offset + 1;              /* Advance one byte */
    if (crlf_is_newline &&                      /* If CRLF is newline & */
	start_offset < subject_length - 1 &&    /* we are at CRLF, */
	subject[start_offset] == '\r' &&
	subject[start_offset + 1] == '\n')
      ovector[1] += 1;                          /* Advance by one more. */
    else if (utf8)                              /* Otherwise, ensure we */
      {                                         /* advance a whole UTF-8 */
      while (ovector[1] < subject_length)       /* character. */
	{
	if ((subject[ovector[1]] & 0xc0) != 0x80) break;
	ovector[1] += 1;
	}
      }
    continue;    /* Go round the loop again */
    }

  /* Other matching errors are not recoverable. */

  if (rc < 0)
    {
	    fprintf(stderr,"Matching error %d\n", rc);
	    pcre_free(re);    /* Release memory used for the compiled pattern */
	    return ;
    }


  /* The match succeeded, but the output vector wasn't big enough. */

  if (rc == 0)
    {
	    rc = OVECCOUNT/3;
	    fprintf(stderr,"ovector only has room for %d captured substrings\n", rc - 1);
    }

  /* show substrings stored in the output vector by number, and then
  also any named substrings. */

	 {
	 	char urlin[512];
	 	char urlout[512];
		char *substring_start = subject + ovector[2];
		int substring_length = ovector[3] - ovector[2];
		//fprintf(stderr,"%.*s\n", substring_length, substring_start);
		sprintf(urlin,"%.*s\n", substring_length, substring_start);
		if(analyseUrl(msg,urlin,urlout,result)==1)
		{
			puts("queueurl");
			puts(urlout);
		//	fprintf(stderr, "%d\n",strlen(urls) );
			strcat(urls,urlout);
			strcat(urls,"\r\n");
		}
		puts("newreg");
	  }
 
}  
pcre_free(re);       /* Release memory used for the compiled pattern */
}
#endif
