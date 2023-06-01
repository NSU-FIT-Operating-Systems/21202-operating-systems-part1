run:
	gcc -shared -fpic hello_dyn.c -o libhello-dyn.so
	gcc hello.c -o hello
	./hello

clean:
	rm *.so hello

all: run
