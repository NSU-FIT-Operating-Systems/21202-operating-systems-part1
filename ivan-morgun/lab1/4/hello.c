#include <stdio.h>
#include <dlfcn.h>

int main() {
    void (*lib_handle) = dlopen("libhello_dyn_runtime_lib.so", RTLD_LAZY);
    if (!lib_handle) {
        printf("Failed to load library: %s\n", dlerror());
        return 1;
    }

    void (*hello_function)() = dlsym(lib_handle, "hello_from_dyn_runtime_lib");
    if (!hello_function) {
        printf("Failed to find function: %s\n", dlerror());
        return 1;
    }

    hello_function();

    dlclose(lib_handle);
    return 0;
}
