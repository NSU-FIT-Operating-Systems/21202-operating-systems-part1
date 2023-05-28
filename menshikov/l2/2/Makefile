all:
	gcc -m64 -c ahello.s -o ahello.o
	ld -m elf_x86_64 -e _start ahello.o -o ahello
clean:
	rm -rf *.o ahello
run:
	./ahello
