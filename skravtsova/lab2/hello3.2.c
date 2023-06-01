#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

void print_hello() {
    const char* message = "Hello world\n";
    syscall(SYS_write, 1, message, strlen(message));
}

int main() {
    print_hello();
    return 0;
}