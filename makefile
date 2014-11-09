Crawler:cra_main.o crawler.o threadpool.o
	cc cra_main.o crawler.o threadpool.o -o Crawler -lpthread

cra_main.o:cra_main.c
	cc -c cra_main.c -o cra_main.o

crawler.o:crawler.c
	cc -c crawler.c -o crawler.o

threadpool.o:threadpool.c
	cc -c threadpool.c -o threadpool.o
