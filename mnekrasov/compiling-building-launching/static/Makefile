CC=gcc
CFLAGS=-Werror

all: exe

exe: main.c libstatic.a
	$(CC) $(CFLAGS) main.c -lstatic -L. -o exe

libstatic.a: static.o
	ar rc libstatic.a static.o

static.o: static.h static.c
	$(CC) $(CFLAGS) -c static.c -o static.o

run: exe
	./exe

clean: static.o libstatic.a exe
	rm -rf static.o libstatic.a exe
