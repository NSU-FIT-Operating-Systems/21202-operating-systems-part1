CC=gcc
CFLAGS=-Werror

all: exe

exe: main.c libshared.so
	$(CC) $(CFLAGS) main.c -o exe -lshared -L.

libshared.so: shared.h shared.c
	$(CC) $(CFLAGS) shared.c -o libshared.so -shared

run: exe
	LD_LIBRARY_PATH=./ ./exe

clean: libshared.so exe
	rm -rf libshared.so exe
