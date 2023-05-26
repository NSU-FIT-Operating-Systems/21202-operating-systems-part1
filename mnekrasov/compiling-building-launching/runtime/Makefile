CC=gcc
CFLAGS=-Werror

LIBPATH=./libruntime.so
FUNCNAME=printHello

all: exe

exe: main.c libruntime.so
	$(CC) $(CFLAGS) main.c -o exe

libruntime.so: runtime.h runtime.c
	$(CC) $(CFLAGS) runtime.c -o libruntime.so -shared

run: exe
	./exe $(LIBPATH) $(FUNCNAME)

clean: libruntime.so exe
	rm -rf libruntime.so exe
