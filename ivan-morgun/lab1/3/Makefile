build_lib:
	gcc -c hello_dynamic_lib.c && gcc -shared -fpic -o libhello_dynamic_lib.so hello_dynamic_lib.o

build:
	gcc -c -Wall -o hello.o hello.c && gcc -o hello hello.o -L. -lhello_dynamic_lib -Wl,-rpath=$(pwd)

run:
	./hello

nm:
	nm hello


# вопрос про секции в elf 
