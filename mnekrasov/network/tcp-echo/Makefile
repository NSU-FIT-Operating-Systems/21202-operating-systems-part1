CC=gcc
CFLAGS=-Wall -Werror -Wpedantic

all: server client

server: server.c
	$(CC) $(CFLAGS) server.c -o server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

clean: server client
	rm -rf server client
