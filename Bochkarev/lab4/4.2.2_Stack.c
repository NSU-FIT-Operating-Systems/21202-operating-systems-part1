#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void addStack()
{
	char a[4096];
	sleep(1);
	addStack();
}

void f1()
{
	addStack();
}

int main()
{
	printf("pid: %d\n", getpid());
	sleep(15);
	
	f1();
	return 0;
}

