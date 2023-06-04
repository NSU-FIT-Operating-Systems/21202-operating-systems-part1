#include <stdio.h>

int main(int argc, char* argv[]) {
	
    const char *message = "Hello, world!\n";
    size_t messageLen = 14;
    
    asm volatile (
        "movq $1, %%rax\n"    // syscall number for write
        "movq $1, %%rdi\n"    // file descriptor 1 (stdout)
        "movq %0, %%rsi\n"    // message address
        "movq %1, %%rdx\n"    // message length
        "syscall\n"           // call the system call
        :
        : "r"(message), "r"(messageLen)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
    
    return 0;
}