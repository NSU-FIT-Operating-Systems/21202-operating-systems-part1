#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#define PAGE_SIZE 4096

void incr_stack() {
	char arr[PAGE_SIZE];
	usleep(10000L);
	incr_stack();
}

int main(int argc, char const *argv[])
{
	printf("pid: %d\n", getpid());
	sleep(15);
	incr_stack();
	return 0;
}