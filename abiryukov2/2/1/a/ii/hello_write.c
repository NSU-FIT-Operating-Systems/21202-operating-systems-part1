#include <sys/syscall.h>
#include <unistd.h>

int main() {
  syscall(SYS_write, STDOUT_FILENO, "Hello world\n", 13);
  return 0;
}
