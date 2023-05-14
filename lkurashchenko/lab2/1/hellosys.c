#include <sys/syscall.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	syscall(SYS_write, 1, "Hello, world!\n", 14);

}
