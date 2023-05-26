#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <malloc.h>

#define SIZE 1024*1024
#define DELAY_BEFORE_START_PROGRAM 15
#define RECURSION_DELAY_IN_MICS 10000

void incr_heap() {
	char *buffer = (char *)malloc(SIZE);
	usleep(RECURSION_DELAY_IN_MICS);
	incr_heap();
	free(buffer);
}

int main(int argc, char const *argv[])
{
	printf("pid: %d\n", getpid());
	sleep(DELAY_BEFORE_START_PROGRAM);
	incr_heap();
	return 0;
}
