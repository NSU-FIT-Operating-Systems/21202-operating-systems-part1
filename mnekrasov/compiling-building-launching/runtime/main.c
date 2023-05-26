#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void loadLibAndRunFunc(char* libPath, char* funcName) {
    void *libBeginning;
    void (*targetFunction)(void);
    char *errorMessage;

    libBeginning = dlopen(libPath, RTLD_LAZY);
    if (!libBeginning) {
        printf("dlopen() failed: %s\n", dlerror());
        return;
    }

    targetFunction = (void (*)(void)) dlsym(libBeginning, funcName);
    errorMessage = dlerror();
    if (errorMessage != NULL) {
        printf("dlsym() failed: %s\n", errorMessage);
        dlclose(libBeginning);
        return;
    }

    targetFunction();
    dlclose(libBeginning);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Use ./exe <lib path> <func name> to run the program\n");
        return EXIT_FAILURE;
    }

    loadLibAndRunFunc(argv[1], argv[2]);

    return EXIT_SUCCESS;
}
