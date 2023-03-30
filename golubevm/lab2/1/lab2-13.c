#include <sys/syscall.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  syscall(SYS_write, 1, "Hello, world!\n", 14);
  return 0;
}
