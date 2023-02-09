CC=gcc
CFLAGS=-c -Wall

all:hello

hello: hello.o
	$(CC) hello.o -o hello
hello.o: hello.c
	$(CC) $(CFLAGS) hello.c
clean:
	rm -rf *.o hello
nm: hello
	nm hello| grep U
ldd: hello
	ldd hello
run: hello
	./hello