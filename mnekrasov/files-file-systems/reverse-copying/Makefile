CC=gcc
CFLAGS=-Werror -Wall -Wpedantic

all: exe

exe: main.c
	$(CC) $(CFLAGS) main.c -o exe

clean: exe
	rm -rf exe
