#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
	write (1, "Hello world from write", 22);
	return 0;
} 
