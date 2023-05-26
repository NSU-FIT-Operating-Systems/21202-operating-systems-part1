#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define STDOUT 1
#define SUCCESS 0

ssize_t writeWrapper(int fd, const void* buf, size_t count) {
    return syscall(SYS_write, fd, buf, count);
}

void printHello() {
    long returned = writeWrapper(STDOUT, "Hello, world!\n", 14);

    if (errno != SUCCESS) {
        perror("writeWrapper() failed");
    } else {
        printf("writeWrapper() returned %ld\n", returned);
    }
}

int main() {
    printHello();

    return EXIT_SUCCESS;
}
