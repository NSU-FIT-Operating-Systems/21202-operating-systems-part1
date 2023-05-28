#include <unistd.h>
#include <sys/syscall.h>
static size_t mywrite(size_t fd, const char* buf, size_t len) {
    return syscall(SYS_write, fd, buf, len);
}

int main() {
    mywrite(1, "my write P.I.M.P", 18);
}
