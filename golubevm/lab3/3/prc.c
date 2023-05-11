#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    pid_t pid = getpid();
    while(1) {
       printf("pid:%d,address: %p\n", pid, &pid);
       sleep(20);
    }
    return 0;
}