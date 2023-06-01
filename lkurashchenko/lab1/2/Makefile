CC=gcc
CFLAGS=-c -Wall
SOURCES=hello.c
OBJECTS=$(SOURCES:.c=.o)

all: lib run

lib: archive

archive: hello_static.o
	ar -rv libhello_static.a hello_static.o

hello_static.o: hello_static.c
	$(CC) $(CFLAGS) hello_static.c

hello: hello.o
	$(CC) hello.o -o hello -L. -lhello_static
	
hello.o: hello.c
	$(CC) $(CFLAGS) hello.c
	
clean:
	rm -rf *.o hello *.a
	
run: hello
	./hello