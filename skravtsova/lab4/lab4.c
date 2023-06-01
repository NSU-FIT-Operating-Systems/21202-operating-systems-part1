#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>

int fourth = 4;
int fifth;
const int sixth = 6;

void params() {
	int first = 1;
	static int second = 2;
	const int third = 3;
	printf("int: %p\nstatic int: %p\nconst int: %p\n", &first, &second, &third);
}

int heap() {
	char *word = (char*) malloc(100 * sizeof(char));
	if (word == NULL) return 1;
	
	strcpy(word, "hello world");

	printf("1: %s\n", word);
	
	free(word);
	
	printf("1: %s\n", word);
	
	char *word2 = (char*) malloc(100 * sizeof(char));
	if (word2 == NULL) return 1;
	
	strcpy(word2, "hello world");

	printf("2: %s\n", word2);
	
	word2 += 50;
	//free(word2);
	
	printf("2: %s\n", word2);
		
	return 0;
}

int main(int argc, char *argv[]) {
	printf("pid %d\n", getpid());
	params();
	printf("global int init: %p\nglobal int not init: %p\nconst int: %p\n", &fourth, &fifth, &sixth);
	if (heap()) perror("heap running error");
	sleep(10);
	if (argc < 3)
	{
		fprintf(stderr, "setenv: Too few arguments\n");
		return 1;
	}
	char *var = getenv (argv[1]);
	if (var == NULL)
	{
		printf("'%s' not found\n", argv[1]);
	}
	else printf("'%s=%s' found\n", argv[1], var);
	
	if (setenv (argv[1], argv[2], 0) != 0)
	{
		fprintf(stderr, "setenv: Cannot set '%s'\n", argv[1]);
	}
	else printf("'%s' changed\n", argv[1]);
	
	var = getenv (argv[1]);
	if (var == NULL)
	{
		printf("'%s' not found\n", argv[1]);
	}
	else printf("'%s=%s' found\n", argv[1], var);
	return 0;
}
