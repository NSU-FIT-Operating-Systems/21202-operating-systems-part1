#include <unistd.h>

int main()
{
	if(write(1, "Hello World!\n", 13) != 13)
	{
		return 1;
	}
	return 0;
}
