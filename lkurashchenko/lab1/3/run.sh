#!/bin/bash
gcc -c -Wall hello.c
gcc -shared -fpic hello_dyn.c -o libhello-dyn.so
gcc hello.o libhello-dyn.so -L. -Wl,-rpath=$(pwd) -o hello
./hello
