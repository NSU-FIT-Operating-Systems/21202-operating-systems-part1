#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void print_function_local_variables_and_their_addr();
uint64_t get_function_local_variable_address();
void allocated_memory();
void get_env();

int global_not_initialized;
int global_initialized = 111;
const int const_global_not_initialized;
const int const_global_initialized = 222;

int main(int argc, char* argv[]) {
	printf("\npid: %d\n", getpid());

	// sleep(10);

    printf("\n==============================\n");
	printf("GLOBAL ADDRESSES\n");
	printf("not initialized:              %p (%d)\n", &global_not_initialized, global_not_initialized);
	printf("initialized:                  %p (%d)\n", &global_initialized, global_initialized);
	printf("const global not initialized: %p (%d)\n", &const_global_not_initialized, const_global_not_initialized);
	printf("const global initialized:     %p (%d)\n", &const_global_initialized, const_global_initialized);

	print_function_local_variables_and_their_addr();

	uint64_t ptr = get_function_local_variable_address();
    printf("\n==============================\n");
	printf("FROM FUNCTION LOCAL VARIABLE\n");
	printf("address of local variable from function:  0x%lx\n", ptr);

	// sleep(100);
	
    get_env();
	allocated_memory();

	return 0;
}

void print_function_local_variables_and_their_addr() {
    printf("\n==============================\n");
	printf("LOCAL FUNCTION ADDRESSES:\n");
	int local_initialized = 333;
	int local_not_initialized;
	const int const_local = 444;
	static int static_initialized = 555;
	static int static_not_initialized;
	static const int static_const = 666;

	printf("local initialized:      %p (%d)\n", &local_initialized, local_initialized);
	printf("local not initialized:  %p (%d)\n", &local_not_initialized, local_not_initialized);
	printf("const local:            %p (%d)\n", &const_local, const_local);
	printf("static initialized:     %p (%d)\n", &static_initialized, static_initialized);
	printf("static not initialized: %p (%d)\n", &static_not_initialized, static_not_initialized);

}

uint64_t get_function_local_variable_address() {
	int a = 1;
	uint64_t b = (uint64_t) &a;
	return b;
}

void allocated_memory() {
    printf("\n==============================\n");
	printf("ALLOCATED MEMORY\n");

	char *buffer1 = (char*) malloc(100);
	strcpy(buffer1, "Hello from buffer1!");
	printf("buffer1 value before free:  %s\n", buffer1);
	free(buffer1);
	printf("buffer1 value after free:   %s\n", buffer1);

	char *buffer2 = (char*) malloc(100);
	strcpy(buffer2, "Hello from buffer2!");
	printf("buffer2 value before free:  %s\n", buffer2);
	buffer2 += 50;
	free(buffer2);
	printf("buffer2 after free:         %s\n", buffer2);
}

void get_env() {
    printf("\n==============================\n");
	printf("GET ENV\n");
	const char *key = "LABVAR";
	printf("variable %s=%s\n", key, getenv(key));
	setenv(key, "UPDATED_VALUE", 1);
	printf("variable after update: %s=%s\n", key, getenv(key));
}