#include <unistd.h>
#include <stdlib.h>

void printHello() {
    write(1, "Hello, world!\n", 14);	
}

int main() {
    printHello();

    return EXIT_SUCCESS;
}
