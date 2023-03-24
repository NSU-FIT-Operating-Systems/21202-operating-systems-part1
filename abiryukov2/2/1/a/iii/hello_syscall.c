#include <unistd.h>

void print_hello() {
  write(STDOUT_FILENO, "Hello world\n", 13);
}

int main() {
  print_hello();
  return 0;
}
