build_lib:
	gcc -c hello_dyn_runtime_lib.c && gcc -shared -fPIC -o libhello_dyn_runtime_lib.so hello_dyn_runtime_lib.o

build:
	gcc -Wall -o hello hello.c -ldl

run:
	LD_LIBRARY_PATH="./" ./hello
