#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 20;

int main()
{
	int local = 10;
	printf("local: %d %p\nglobal: %d %p\n", local, &local, global, &global);
	printf("parent pid: %d\n", getpid());

	int pid = fork();
	
	if(pid < 0)
	{
		printf("error creating process\n");
		return 0;
	}
	
	if(pid > 0)
	{
		printf("child pid %d\n", pid);
		sleep(5);
	}
	sleep(10);
	if(pid == 0)
	{
		printf("(child) pid: %d parent pid: %d\n", getpid(), getppid());
		printf("(child) local: %d %p global: %d %p\n", local, &local, global, &global);
		local = 5;
		global = 7;
		printf("(child - changed) local: %d %p global: %d %p\n", local, &local, global, &global);
		sleep(7);
	}
	if(pid > 0)
	{
		printf("(parent - changed in child) local: %d %p global: %d %p\n", local, &local, global, &global);
		sleep(10);		
	}	
	if(pid == 0)
	{
		exit(5);
	}
	if(pid > 0)
	{
		int* status;
		int result = waitpid(pid, status, 0);
		if(WIFEXITED(*status))
		{
			printf("Child process exited with %d status\n", *status);
			return 0;
		}
		else
		{
			printf("Child process did not exit successfully\n");
			return 0;
		}

	}
	return 0;
}
