CC =gcc
CFLAGS=-c -Wall
CFILE=hello_static.c
LIB=static.a
OBJECT=hello.o
MAIN=hello.c

compile: $(CFILE)
	$(CC) $(CFLAGS) $(MAIN)

lib: compile
	ar -rv $(LIB) $(OBJECT)

build: lib
	$(CC) hello.o -o hello -L -lstatic

clean:
	rm -f *.o *.a hello 

nm:build
	$(info hello_static in hello:)
	nm hello | grep hello_static

run: build nm
	$(info ========Executing========)
	./hello
