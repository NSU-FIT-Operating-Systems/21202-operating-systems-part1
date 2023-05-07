#include <unistd.h>
#include <sys/syscall.h>

int print(const char* buf)
{	
	int count = 0;
	for(int i = 0; buf[i] != '\0'; i++)
	{
		count++;
	}
	register int    syscall_no  asm("rax") = 1;
	register int    arg1        asm("rdi") = 1;
	register char*  arg2        asm("rsi") = buf;
	register int    arg3        asm("rdx") = count;
	asm("syscall");
	return 0;
	
}

int main()
{
	print("Hello world!\n");
	return 0;
}
