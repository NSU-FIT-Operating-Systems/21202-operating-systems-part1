CC =gcc
CFLAGS=-c -Wall

all: run

compile: hello.c
	$(CC) $(CFLAGS) hello.c

build: compile
	$(CC) hello.o -o hello

nm: build
	nm hello| grep U >>nm.txt
	$(info ================nm results saved in nm.txt, undefined:)
	cat nm.txt
ldd: compile
	ldd hello >>ldd.txt
	$(info =================dependencies in ldd.txt, they are:)
	cat ldd.txt

clean:
	rm -rf *.o hello *.txt

run: clean nm ldd
	./hello
