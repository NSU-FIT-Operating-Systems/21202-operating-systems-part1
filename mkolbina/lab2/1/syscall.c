#include <stdio.h>
#include <sys/syscall.h>
#include <errno.h>
#include <unistd.h>

static ssize_t MyWrite(int fd, const void *buf, size_t count) {
	return syscall(SYS_write, fd, buf, count);
}

int main() {
	int ret = MyWrite(1, "Hello syscall", 13);
	return 0;

}
