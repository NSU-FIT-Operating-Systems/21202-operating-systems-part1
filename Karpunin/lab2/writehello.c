#include <string.h>
#include <unistd.h>

void print_hello(void) {
	const char msg[] = "Hello world!";
	write(STDOUT_FILENO, msg, strlen(msg));
}

int main() {
	print_hello();
	return 0;
}
