#include <stdio.h>
#include <dlfcn.h>

void print_hello(void) {
	printf("Hello from hello.c!\n");
}

int main() {
	void* lib_handler;
	void (*print_runtime_hello)(void);
	lib_handler = dlopen("~/OSlabs/lab1/libdynamic_hello.so", RTLD_LAZY);
	void (*print_dyn_runtime_hello)(void);
	print_runtime_hello =  (void (*)(void)) dlsym(lib_handler, "print_dyn_runtime_hello");

	print_hello();
	print_static_hello();
	print_dyn_hello();
	print_runtime_hello();
	return 0;
}
