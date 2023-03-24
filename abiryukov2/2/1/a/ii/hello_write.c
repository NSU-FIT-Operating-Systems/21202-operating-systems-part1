#include <unistd.h>

int main() {
  write(STDOUT_FILENO, "Hello world\n", 13);
  return 0;
}
