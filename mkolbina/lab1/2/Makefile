CC=gcc
CFLAGS=-c -Wall
STATIC_LIB_NAME=libprint_hello_static.a
STATIC_LIB_SOURCES=print_hello_static.c
STATIC_LIB_OBJECTS= $(STATIC_LIB_SOURCES:.c=.o)
SOURCES=hello.c
OBJECTS=$(SOURCES:.c=.o)

all: staticLib run

staticLib: archive

archive: $(STATIC_LIB_OBJECTS)
	ar -rv $(STATIC_LIB_NAME) $(STATIC_LIB_OBJECTS)

$(STATIC_LIB_OBJECTS): $(STATIC_LIB_SOURCES)
	$(CC) $(CFLAGS) $(STATIC_LIB_SOURCES)

hello: hello.o
	$(CC) hello2.o -o hello2 -L. -lprint_hello_static

hello.o: hello2.c
	$(CC) $(CFLAGS) hello2.c

clean:
	rm -rf *.o hello *.a

run: hello
	./hello2