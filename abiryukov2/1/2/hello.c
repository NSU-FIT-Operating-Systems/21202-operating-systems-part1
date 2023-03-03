#include <stdio.h>
#include "hello_static.h"

int main() {
    printf("Hello world\n");
    printf("main is : %p\n", &main);
    hello_from_static_lib();
}
