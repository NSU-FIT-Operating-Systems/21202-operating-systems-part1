#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

void mywrite(int fd, void *buf, size_t count) {
    syscall(SYS_write, fd, buf, count);
}

int main(int argc, char* argv[]) {
	mywrite(1, "Hello, world!\n", 14);
	return 0;
}