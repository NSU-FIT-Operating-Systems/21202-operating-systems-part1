#include <stdio.h>
#include "hello_dyn.h"
#include <unistd.h>
int main(int argc, char* argv[]) {
	printf("Hello, world!\n");
	int s = 0;
	for(int i = 0; i < 100000000; i++) {
		s+=i;
	}
	printf("%d",s);
	sleep(1000);
	hello_from_dynamic_lib();
}
