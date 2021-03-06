#include<stdio.h>
#include <string.h>
#include <assert.h>
#include <zlib.h>
#include <stdlib.h>

static const int MAX_SOURCE_LEN=1024*1024*10;//max input char number 10 M

#define CHUNK 1024*256//largest ungzip buffer size
int ungzip(char *source,int len,FILE *result)
{
	int  ret; 	
	unsigned int uncom_len; 
	unsigned  char  out[CHUNK]; 
	z_stream strm;
	
	if(!source||len==0)
	return;
		
	strm.zalloc=Z_NULL;
	strm.zfree=Z_NULL;
	strm.opaque=Z_NULL;
	strm.avail_in=0;
	strm.next_in=Z_NULL;

	ret=inflateInit2(&strm,47);

	if(ret!=Z_OK)
	{
		perror("Zlib");
		return -1;
	}

	strm.next_in=source;
	strm.avail_in=len;
	//do { 
		strm.avail_out  =  CHUNK; 
		strm.next_out  =  out; 
		ret  =  inflate(&strm,  Z_NO_FLUSH); 
		assert(ret  !=  Z_STREAM_ERROR);     
		
		switch  (ret)  { 
			case  Z_NEED_DICT: 
			ret  =  Z_DATA_ERROR;    
			puts("Z_data_error");
			case  Z_DATA_ERROR: 
			puts("Z_data error");
			case  Z_MEM_ERROR: 
			puts("mem error");
			inflateEnd(&strm); 
			return  -1; 
		} 
		
		uncom_len =  CHUNK  -  strm.avail_out; 
	//	fprintf(stderr,"uncom_len:%d\n",uncom_len);
	   	fwrite(out,1,uncom_len,result);
	//} while  (strm.avail_out  ==  0); 
	return  ret  ==  Z_STREAM_END  ?  1 :  0; 

}

void show(unsigned char*str,int len,int Gzip)//gzip(1) hex other char
{
	int i;
	for(i=0;i<len;++i)
	{
	  if(Gzip)
	  {	
		  fprintf(stderr,"%2x ",*str++);
		  if((i+1)%10==0)
		  puts("");
	  }
	  else
	  {
		  fprintf(stderr,"%c",*str++);
	  }
	}
}

int uncompressWebPage(FILE *source,FILE* result)
{
	char* buffer=(char*)malloc(MAX_SOURCE_LEN);
	int len=0;
	if(!buffer)
	{
		perror("Malloc");
		return -1;
	}
//filter for the head info,but I found it's useless
	while(fgets(buffer,1024,source))
	{
		if(sscanf(buffer,"Content-Length:%d",&len))
		{
			fprintf(stderr,"length:%d\n",len);
			while(fgets(buffer,1024,source))
			{
				if(strlen(buffer)==2)
				{
			        	break;
				}
			}
			break;
		}
	}
	if(!source || !result)
	{
		puts("File error");
	//	return -1;
	}
	if(len=fread(buffer,1,MAX_SOURCE_LEN,source))
	{
		//fprintf(stderr,"Read len:%d\n",len);
		if(len==MAX_SOURCE_LEN)
		{
			puts("Source too long");
			return -1;
		}
		if(ungzip(buffer,len,result)<0)
		{
			puts("ungzip failed,maybe not a standard gzip content");
			show(buffer,len,1);
			return -1;
		}
		else
		{
			fclose(result);
			fclose(source);
			free(buffer);
			return 1;
		}
		//puts("success");
	}
	return 0;
}
/*
int main()
{
	int i;
	FILE *source;
	FILE *result;
	for(i=0;i<100;i++)
	{
		char tmp_source[24];
		char tmp_result[24];	
		sprintf(tmp_result,"%dresult.html",i);
		sprintf(tmp_source,"%d.html",i);
		source=fopen(tmp_source,"r");
		result=fopen(tmp_result,"w");
		if(!source)
		{
			fprintf(stderr,"%s not exits\n",tmp_source);
			continue;
		}
		uncompressWebPage(source,result);
	}

	return 0;

}
*/
