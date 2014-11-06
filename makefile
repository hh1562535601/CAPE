Crawler:main.o sock.o
	cc main.o sock.o -o Crawler

main.o:main.c
	cc -c main.c -o main.o

sock.o:sock.c
	cc -c sock.c -o sock.o
