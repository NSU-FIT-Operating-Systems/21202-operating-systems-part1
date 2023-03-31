CC=gcc
CFLAGS=-Werror

all: exe

exe: executable.c
	$(CC) $(CFLAGS) executable.c -o exe

run: exe
	./exe arg1 arg2 arg3

clean: exe
	rm -rf exe
