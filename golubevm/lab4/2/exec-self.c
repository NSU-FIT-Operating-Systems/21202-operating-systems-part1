#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

#define DELAY 5

int main(int argc, char const *argv[])
{
	printf("Hello, world! I'm %d\n", getpid());
	sleep(DELAY);
	execv(argv[0], argv);
	return 0;
}
