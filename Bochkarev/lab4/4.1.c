#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int d = 10;
int e;
const int f = 10;

int* f1()
{
	int a = 10;
	return &a;
}

void f2()
{
	char* a = (char*) malloc(100);
	a[0] = 'h';
	a[1] = 'e';
	a[2] = 'l';
	a[3] = 'l';
	a[4] = 'o';
	a[5] = ' ';
	a[6] = 'w';
	a[7] = 'o';
	a[8] = 'r';
	a[9] = 'l';
	a[10] = 'd';
	a[11] = '\0';
	for(int i = 0; i < 100; i++)
	{
		printf("%c", a[i]);
	}
	printf("\n");
	free(a);
	for(int i = 0; i < 100; i++)
	{
		printf("%c", a[i]);
	}
	printf("\n");

	char* b = (char*) malloc(100);
	b[0] = 'h';
	b[1] = 'e';
	b[2] = 'l';
	b[3] = 'l';
	b[4] = 'o';
	b[5] = ' ';
	b[6] = 'w';
	b[7] = 'o';
	b[8] = 'r';
	b[9] = 'l';
	b[10] = 'd';
	b[11] = '\0';

	for(int i = 0; i < 100; i++)
	{
		printf("%c", b[i]);
	}
	printf("\n");

	b = &b[50];
	free(b);
	for(int i = 0; i < 100; i++)
	{
		printf("%c", b[i - 50]);
	}
	printf("\n");

}

void f3()
{
	char* arr = getenv("PORT");
	printf("%s\n", arr);

	putenv("PORT=2000");

	arr = getenv("PORT");
	printf("%s\n", arr);
}

int main()
{

	int a = 10;
	static int b = 10;
	const int c = 10;

	int pid = getpid();

	printf("pid: %d\n", pid);
	printf("local func: %p\n", &a);
	printf("static func: %p\n", &b);
	printf("const func: %p\n", &c);
	printf("global uninit: %p\n", &e);
	printf("global init: %p\n", &d);
	printf("global const: %p\n", &f);

	int* nothing = f1();
	printf("local from func: %p\n", nothing);
	f3();
	sleep(30);

	return 0;
}
