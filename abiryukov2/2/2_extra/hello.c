#include <stdio.h>
#include <time.h>
#include <sys/syscall.h>
#include <unistd.h>

#define N 100000

unsigned long long TimePrintf() {
    
    const char msg[] = "Hello, world\n";
    size_t len = sizeof(msg) - 1;

    unsigned long long start, end, elapsed;
    int i;

    // Measure the time in ticks
    asm volatile ("rdtsc\n\t" // Get the initial time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (start) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Loop to execute the assembly code multiple times
    for (i = 0; i < N; i++) {
        printf("Hello world\n");
    }

    asm volatile ("rdtsc\n\t" // Get the final time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (end) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Calculate the elapsed time in ticks
    elapsed = end - start;

    return elapsed;
}

unsigned long long TimeSyscall() {
    
    const char msg[] = "Hello, world\n";
    size_t len = sizeof(msg) - 1;

    unsigned long long start, end, elapsed;
    int i;

    // Measure the time in ticks
    asm volatile ("rdtsc\n\t" // Get the initial time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (start) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Loop to execute the assembly code multiple times
    for (i = 0; i < N; i++) {
        syscall(SYS_write, STDOUT_FILENO, "Hello world\n", 13);
    }

    asm volatile ("rdtsc\n\t" // Get the final time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (end) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Calculate the elapsed time in ticks
    elapsed = end - start;

    return elapsed;
}

unsigned long long TimeWrite() {
    
    const char msg[] = "Hello, world\n";
    size_t len = sizeof(msg) - 1;

    unsigned long long start, end, elapsed;
    int i;

    // Measure the time in ticks
    asm volatile ("rdtsc\n\t" // Get the initial time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (start) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Loop to execute the assembly code multiple times
    for (i = 0; i < N; i++) {
        write(STDOUT_FILENO, "Hello world\n", 13);
    }

    asm volatile ("rdtsc\n\t" // Get the final time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (end) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Calculate the elapsed time in ticks
    elapsed = end - start;

    return elapsed;
}

unsigned long long TimeSyscallByAsm() {
    
    const char msg[] = "Hello, world\n";
    size_t len = sizeof(msg) - 1;

    unsigned long long start, end, elapsed;
    int i;

    // Measure the time in ticks
    asm volatile ("rdtsc\n\t" // Get the initial time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (start) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Loop to execute the assembly code multiple times
    for (i = 0; i < N; i++) {
        asm volatile (
            "movq $1, %%rax\n\t"
            "movq $1, %%rdi\n\t"
            "movq %0, %%rsi\n\t"
            "movq %1, %%rdx\n\t"
            "syscall\n\t"
            // "movq $60, %%rax\n\t"
            // "xorq %%rdi, %%rdi\n\t"
            // "syscall\n\t"
            :
            : "r"(msg), "r"(len)
            : "%rax", "%rdi", "%rsi", "%rdx"
        );
    }

    asm volatile ("rdtsc\n\t" // Get the final time
                  "shlq $32, %%rdx\n\t"
                  "orq %%rax, %%rdx\n\t"
                  "movq %%rdx, %0\n\t"
                  : "=r" (end) // output
                  : // no input
                  : "%rax", "%rdx"); // clobbered registers

    // Calculate the elapsed time in ticks
    elapsed = end - start;

    return elapsed;

}


int main() {
    printf("Elapsed time:\n\tprintf:\t\t%llu\n\tsyscall:\t%llu\n\twrite:\t\t%llu\n\tby asm:\t\t%llu\n", 
    TimePrintf(), TimeSyscall(), TimeWrite(), TimeSyscallByAsm());

    return 0;
}
