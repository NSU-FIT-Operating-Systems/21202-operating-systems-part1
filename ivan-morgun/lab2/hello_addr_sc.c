#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define ksys_write_addr 0xffffffff8130cf40 

typedef size_t (*ksys_write_t)(int fd, void *buf, size_t count);

int main(int argc, char* argv[]) {

    ksys_write_t ksys_write = (ksys_write_t)ksys_write_addr;
	ksys_write(1, "Hello, world!\n", 14);
	return 0;
}