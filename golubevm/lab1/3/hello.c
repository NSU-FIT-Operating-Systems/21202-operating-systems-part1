#include<stdio.h>
#include "hello_dyn.h"

int main(int argc, char* argv[]) {
	printf("Hello, world!\n");
	hello_from_dynamic_lib();
}
