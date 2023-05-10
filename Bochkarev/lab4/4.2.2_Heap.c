#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <string.h>

void handler(int sig, siginfo_t* arg1, void* arg2)
{
	write(1, "handled sigsegv\n", strlen("handling sigsegv\n"));
	exit(0);
}

void catchSigSegv(char* p, int block)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_sigaction = &handler;
	sa.sa_flags = SA_SIGINFO;
	if(!sigaction(SIGSEGV, &sa, NULL))
	{
		printf("Sigaction failure\n");
		return;
	}
		
	if(!mprotect(p, block * 10, PROT_NONE))
	{
		printf("Mprotect failure\n");
		return;
	} 
	//p[15] = 'a';
	//char c = p[15];
}

void addHeap()
{
	int block = 1024 * 1024;
	char** start = (char**) malloc(sizeof(char*) * 20);
	
	printf("mallocing 20 blocks...\n");
	for(int i = 0; i < 20; i++)
	{	
		start[i] = malloc(block);
		sleep(1);
	}
	
	printf("20 blocks malloced\n");
	sleep(5);
	
	for(int i = 0; i < 20; i++)
	{
		free(start[i]);
	}
	
	block = 4096; //page size
	printf("20 blocks freed\n");
	sleep(5);
	
	char* p = mmap(NULL, block * 10, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if(p == MAP_FAILED)
	{
		printf("Failed to map memory\n");
		exit(0);
	}
	
	printf("mapped 10 blocks\n");
	sleep(5);
	
	catchSigSegv(p, block);
	
	printf("sigsegv skipped\n");
	sleep(5);
	
	munmap(p + block * 4, block * 2);	
	printf("Done\n");
	sleep(300);
}

void f2()
{
	addHeap();
}

int main()
{
	printf("pid: %d\n", getpid());
	sleep(15);
	
	f2();
	return 0;
}






