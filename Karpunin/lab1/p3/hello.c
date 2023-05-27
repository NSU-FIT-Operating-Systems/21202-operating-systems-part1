#include <stdio.h>
#include <unistd.h>


void print_hello(void) {
	printf("hello!");
}


int main() {
	sleep(100);
	print_hello();
	print_from_dynamic();
}
