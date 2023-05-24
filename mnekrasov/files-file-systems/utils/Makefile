CC=gcc
CFLAGS=-Werror

ARGUMENTS=a b c d e f g h i j k l m n

all: exe

exe: main.c
	$(CC) $(CFLAGS) main.c -o exe

links: exe
	for argument in $(ARGUMENTS); do\
        ln exe $$argument;\
    done

clean: exe
	rm -rf exe $(ARGUMENTS)
