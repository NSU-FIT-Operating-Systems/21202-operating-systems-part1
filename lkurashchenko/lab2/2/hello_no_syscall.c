#include <stdio.h>

int main() {

    const char *hello = "Hello, world!\n";
    size_t hello_len = 14;

    asm volatile (
            "movq $1, %%rax\n" //sys num
            "movq $1, %%rdi\n" //stdout
            "movq %0, %%rsi\n" //buffer
            "movq %1, %%rdx\n"
            "syscall\n"
            ://out
            : "r"(hello), "r"(hello_len) //in
            : "%rax", "%rdi", "%rsi", "%rdx" //changed
            );

    return 0;
}
