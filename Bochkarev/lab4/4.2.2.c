#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

void addStack()
{
	char a[4096];
	addStack();
}

void addHeap()
{
	int size = 0;
	int block = 4096;
	char* start;
	int i = 0;
	while(i++ < 50)
	{
		char* p;
		if(malloc(block));
		size += block;
		sleep(1);
		if(size > block * 20)
		{
			p = mmap(NULL, block, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
			if(size == block)
			{
				start = p;
			}
		}
		/*
		
		if(size > block * 40)
		{
			//mprotect(p, block, PROT_READ); 
			//p[15] = 'a';
			mprotect(p, block, PROT_WRITE);
			char c = p[15];
		}
		*/
	}
	munmap(start + block * 4, block * 2);
}

void f2()
{
	addHeap();
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
