#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
	write(1, "Hello, world!\n", 10);
	return 0;
}