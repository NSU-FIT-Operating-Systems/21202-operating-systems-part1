#include <unistd.h>
#include <sys/syscall.h>

int print(const char* buf)
{
	int count = 0;
	for(int i = 0; buf[i] != '\0'; i++)
	{
		count++;
	}
	int res = syscall(SYS_write, 1, buf, count);
	if(res < 0)
	{
		return -1;
	}
	return res;
}

int main()
{
	print("Hello world!\n");
	return 0;
}
