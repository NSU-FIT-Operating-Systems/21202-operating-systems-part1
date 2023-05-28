#include <unistd.h>
#include <string.h>

void print_hello(void) {
	const char msg[] = "Hello world";
	syscall(1, STDOUT_FILENO, msg, strlen(msg));
}

int main() {
	print_hello();
	return 0;
}
