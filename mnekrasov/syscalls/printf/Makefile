CC=gcc
CFLAGS=-Werror

all: exe

exe: main.c
	$(CC) $(CFLAGS) main.c -o exe

run: exe
	./exe

clean: exe
	rm -rf exe
