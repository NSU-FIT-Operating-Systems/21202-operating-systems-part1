#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int* f() {
	int a = 10;
	return &a;
}


int main() {
	printf("&a = '%p'\n", f());

	char* s = getenv("MYENV");
	s = "OSlabs";
	setenv("MYENV", s, 1);
	printf("New value of MYENV is: %s", getenv("MYENV"));

	return 0;

}
