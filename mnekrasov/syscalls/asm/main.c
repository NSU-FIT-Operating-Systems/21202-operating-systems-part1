#include <stdio.h>
#include <stdlib.h>

void printHello() {
    const char message[] = "Hello, world!\n";
    const size_t messageLength = sizeof(message);
    long returned = 0;

    asm(
        "movq $1, %%rax; \n"
        "movq $1, %%rdi; \n"
        "syscall"
        : "=a" (returned)
        : "S" (message), "d" (messageLength)
    );

    printf("Syscall returned %ld\n", returned);
}

int main() {
    printHello();
    
    return EXIT_SUCCESS;
}
