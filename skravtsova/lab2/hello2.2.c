#include <unistd.h>
#include <string.h>

void print_hello() {
    const char* message = "Hello world\n";
    write(1, message, strlen(message));
}

int main() {
    print_hello();
    return 0;
}