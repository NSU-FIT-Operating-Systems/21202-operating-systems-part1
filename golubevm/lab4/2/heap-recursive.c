#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <malloc.h>

#define SIZE 1024*1024

void incr_heap() {
	char *buffer = (char *)malloc(SIZE);
	usleep(10000L);
	incr_heap();
}

int main(int argc, char const *argv[])
{
	printf("pid: %d\n", getpid());
	sleep(15);
	incr_heap();
	return 0;
}
