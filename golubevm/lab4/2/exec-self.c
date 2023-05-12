#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
	sleep(1);
	execv(argv[0], argv);
	return 0;
}
