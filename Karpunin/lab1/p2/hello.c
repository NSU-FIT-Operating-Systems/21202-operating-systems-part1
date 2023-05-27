#include <stdio.h>
#include "hello_from_static.h"




void print_hello(void) {
	printf("Hello from local!");
}

int main() {
	print_hello();
	print_from_static();
	return 0;
}
