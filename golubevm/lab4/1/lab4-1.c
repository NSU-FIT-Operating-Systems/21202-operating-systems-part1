#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

void print_function_variables_and_their_addr();
uint64_t get_local_address();
void task_with_heap();
void task_with_env();

int global_not_inited_f;
int global_inited_f = 145;
const int const_global_not_inited_f;
const int const_global_inited_f = 100;
char *global_pstr_f = "Hello, global pstr!";
char global_array_not_inited_f[4096];
char global_array_inited_f[] = "Hello, global inited array";

int main(int argc, char* argv[]) {
	printf("\npid: %d\n", getpid());
	// sleep(15);
	printf("#global addresses:\n");
	printf("--global not inited:                       %p (%d)\n", &global_not_inited_f, global_not_inited_f);
	printf("--global inited:                           %p (%d)\n", &global_inited_f, global_inited_f);
	printf("--const global not inited:                 %p (%d)\n", &const_global_not_inited_f, const_global_not_inited_f);
	printf("--const global inited:                     %p (%d)\n", &const_global_inited_f, const_global_inited_f);
	printf("--global pstr:                             %p %p (%s)\n", &global_pstr_f, global_pstr_f, global_pstr_f);
	printf("--global not inited array:                 %p %p (%c)\n", &global_array_not_inited_f, global_array_not_inited_f, *global_array_not_inited_f);
	printf("--global array inited:                     %p %p (%s)\n", &global_array_inited_f, global_array_inited_f, global_array_inited_f);
	print_function_variables_and_their_addr();
	uint64_t ptr = get_local_address();
	printf("#address of local variable from function:  0x%lx\n", ptr);
	// task_with_heap();
	// sleep(300);
	task_with_env();
	// task_with_heap();
	return 0;
}

void print_function_variables_and_their_addr() {
	printf("#function addresses:\n");
	printf("--print_function_variables_and_their_addr: %p\n", print_function_variables_and_their_addr);
	int local_inited_s = 333;
	int local_not_inited_s;
	char local_array_s[] = "Hello, world!";
	char *local_pstr_s = "Hello, local_pstr_s!";
	const int const_local_s = 444;
	static int static_inited_s = 555;
	static int static_not_inited_s;
	static const int static_const_s = 666;

	printf("--local inited:                            %p (%d)\n", &local_inited_s, local_inited_s);
	printf("--local not inited:                        %p (%d)\n", &local_not_inited_s, local_not_inited_s);
	printf("--local array:                             %p %p (%s)\n", &local_array_s, &local_array_s[0], local_array_s);
	printf("--local pstr:                              %p %p (%s)\n", &local_pstr_s, &local_pstr_s[0], local_pstr_s);
	printf("--const local:                             %p (%d)\n", &const_local_s, const_local_s);
	printf("--static inited:                           %p (%d)\n", &static_inited_s, static_inited_s);
	printf("--static not inited:                       %p (%d)\n", &static_not_inited_s, static_not_inited_s);
	printf("--static const:                            %p %d\n", &static_const_s, static_const_s);

}

uint64_t get_local_address() {
	int a = 555;
	uint64_t b = (uint64_t)&a;
	return b;
}

void task_with_heap() {
	printf("#task with heap\n");
	char *buffer1 = (char *)malloc(100);
	strcpy(buffer1, "Hello, buffer1!");
	printf("--buffer1 value before free:  %s\n", buffer1);
	free(buffer1);
	printf("--buffer1 value after free:   %s\n", buffer1);
	char *buffer2 = (char *)malloc(128);
	strcpy(buffer2, "Hello, buffer2!");
	printf("--buffer2 value before free:  %s\n", buffer2);
	buffer2 += 64;
	free(buffer2);
	printf("--buffer2 after shifted free: %s\n", buffer2);
}

void task_with_env() {
	const char *key = "TEST_KEY";
	printf("#task with env\n");
	printf("--before upd: %s=%s\n", key, getenv(key));
	setenv(key, "UPDATED_VALUE", 1);
	printf("--after update: %s=%s\n", key, getenv(key));
}
