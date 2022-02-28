CC=gcc
CFLAGS=-c -Wall -g 

all: stats

stats: main.o 
	$(CC) main.o -o main


main.o: main.c
	$(CC) $(CFLAGS) main.c

clean:
	/bin/rm -f main *.o *.gz

run:
	./main

tarball:
	tar -cvzf Riddick.tar.gz main.c README.txt Makefile
