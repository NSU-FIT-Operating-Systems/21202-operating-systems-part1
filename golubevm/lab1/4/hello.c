#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>


int main(int argc, char* argv[]){
	printf("Hello, world!\n");
	
	void* handle;
	void (*hello_dyn)(void);
	char* error;
	
	handle = dlopen("libhello-dyn-run.so", RTLD_LAZY);
	
	if (!handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
		dlclose(handle);
        return EXIT_FAILURE;
    }
	
	dlerror();
	hello_dyn = (void (*)(void)) dlsym(handle, "hello_from_dyn_runtime_lib");
	error = dlerror();
	
	if (error != NULL) {
		fprintf(stderr, "Error: %s\n", error);
		dlclose(handle);
		return EXIT_FAILURE;
	}
	hello_dyn();	
	dlclose(handle);
}
