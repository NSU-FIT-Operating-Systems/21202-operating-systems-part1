#include <stdio.h>
#include "hello_static.h"

int main(int argc, char* argv[]) {
	printf("Hello, world!\n");
	hello_from_static_lib();
}
