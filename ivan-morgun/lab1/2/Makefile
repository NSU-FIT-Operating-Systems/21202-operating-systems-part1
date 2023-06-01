build-lib:
	gcc -c hello_static_lib.c

archive:
	ar rcs libhello_static_lib.a hello_static_lib.o

build:
	gcc -o hello hello.c -L. -lhello_static_lib

nm:
	nm -u hello
	