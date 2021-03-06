Crawler:cra_main.o crawler.o
	cc -g cra_main.o crawler.o -o Crawler -levent

cra_main.o:cra_main.c
	cc -g -c cra_main.c

crawler.o:crawler.c
	cc -g -c crawler.c
	
ipc_nano.o:ipc_nano.c
	cc -g -c ipc_nano.c

Analyzer:ana_main.o  analyzer.o pcre.o BloomFilter.o GeneralHashFunctions.o
	cc -g ana_main.o analyzer.o pcre.o BloomFilter.o GeneralHashFunctions.o -o Analyzer
	
ana_main.o:ana_main.c
	cc -g -c ana_main.c
	
analyzer.o:analyzer.c
	cc -g -c analyzer.c
	
pcre.o:pcre.c
	cc -g -c pcre.c
	
BloomFilter.o:BloomFilter.c
	cc -g -c BloomFilter.c

GeneralHashFunctions.o:GeneralHashFunctions.c
	cc -c -g GeneralHashFunctions.c
