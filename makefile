Crawler:cra_main.o crawler.o threadpool.o ipc_nano.o
	cc cra_main.o crawler.o threadpool.o ipc_nano.o -o Crawler -lpthread -lnanomsg -levent

Analyzer:ana_main.o  analyzer.o ipc_nano.o ungzip.o
	cc ana_main.o analyzer.o ipc_nano.o ungzip.o -o Analyzer -lnanomsg -lz
	
