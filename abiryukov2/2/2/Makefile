all:
	gcc -m64 -c hello.S -o hello.o
	ld -m elf_x86_64 -e _start hello.o -o hello
	./hello
