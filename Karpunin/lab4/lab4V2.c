#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int global_not_inited[8192];
int global_inited = 10;

const int global_const = 20;

void f(void) {
	int my_int;
	static int my_static_int;
	const int my_const_int;

	printf("Local int: %p (%d)\n", &my_int, my_int);
	printf("Static int: %p (%d)\n", &my_static_int, my_static_int);
	printf("Const int: %p (%d)\n", &my_const_int, my_const_int);
}


void ff(void) {
	char* pstr = (char*) malloc(100);
	strcpy(pstr, "hello world");
	printf("pstr = '%s'\n", pstr);
	pstr += 50;
	free(pstr);
	printf("pstr = '%s'\n", pstr);
}

int main() {
	printf("Global not inited: %p (%d)\n", global_not_inited, global_not_inited[8100]);
	printf("Global inited: %p (%d)\n", &global_inited, global_inited);
	printf("Global const: %p (%d)\n", &global_const, global_const);

	f();

	printf("PID: %d\n", getpid());

	ff();

	sleep(40);
}
