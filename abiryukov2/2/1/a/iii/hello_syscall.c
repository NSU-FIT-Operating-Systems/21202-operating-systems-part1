#include <sys/syscall.h>
#include <unistd.h>

void my_write(int fd, const void *buf, size_t count) {
  syscall(SYS_write, fd, buf, count);
}

int main() {
  my_write(STDOUT_FILENO, "Hello world\n", 13);
  return 0;
}
