#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	printf("pid: %d\n", getpid());
	sleep(1);
	execl("./a.out", " ", NULL);
	printf("Hello world!\n");
	return 0;
}
