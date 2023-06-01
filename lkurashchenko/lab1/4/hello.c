#include <dlfcn.h>
#include <stdio.h>

int main() {
    void *handle = dlopen("./libhello-dyn.so", RTLD_LAZY);

    if (!handle) {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
        return 1;
    }

    void (*dl_hello)() = dlsym(handle, "hello_dyn");
    char *error = dlerror();
    if (error != NULL) {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
        return 1;
    }

    (*dl_hello)();

    dlclose(handle);

}