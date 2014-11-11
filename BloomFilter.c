#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>

#include "GeneralHashFunctions.h"

#define HASH_NUMBER 11
#define BIT_ARRAY_LENGTH (2*20*10000)

void bf_add(void *, char* );
int bf_search(void *bf, char *line);
void set_BIT(unsigned int bit, void *bf);
unsigned int get_BIT(unsigned int bit, void *bf);
unsigned int (*hashfamily[HASH_NUMBER])(char *, unsigned int) = {&RSHash, &JSHash, &PJWHash, &ELFHash, &BKDRHash, &SDBMHash, &DJBHash, &DEKHash, &BPHash, &FNVHash, &APHash};


void bf_add(void *bf, char *str)
{
	int i = 0;
	while(i<HASH_NUMBER)//hash_number=11
	{
		set_BIT((*hashfamily[i++])(str, strlen(str)), bf);
	}
}

int bf_search(void *bf, char *str)
	int i = 0;
	while(i<HASH_NUMBER)
		if(get_BIT(hashfamily[i++](str, strlen(str)), bf) == 0)
			return 0;
	return 1;
}

void set_BIT(unsigned int bit, void *bf)//置位函数
{
	unsigned int i = bit/8%BIT_ARRAY_LENGTH;
	int j = bit%8;
	char *c = (char *) bf;
	//printf("i: %d, j: %d\n", i, j);
	//printf("c[i] : %2x\n", c[i]);
	c[i] = (0x00000001<<j) | c[i];
	//printf("c[i] : %2x\n", c[i]);
}

unsigned int get_BIT(unsigned int bit, void *bf)//检察标志位
{
	unsigned int i = bit/8%BIT_ARRAY_LENGTH;
	int j = bit%8;
	char *c = (char *) bf;
	if ((int)(c[i] & (0x00000001<<j)) == 0)
		return 0;
	else 
		return 1;
}



