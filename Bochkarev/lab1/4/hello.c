#include <stdio.h>
#include <dlfcn.h>

int main() 
{
    void *lib_ptr = dlopen("./lib.so", RTLD_LAZY);
    
    if (!lib_ptr) 
    {
        fputs(dlerror(), stderr);
        return(1);
    }
    
    char *error;
    void (*func)();
    func = dlsym(lib_ptr, "hello_from_dyn_runtime_lib");
    if ((error = dlerror()) != NULL)  
    {
        printf("%s\n", error);
        dlclose(lib_ptr);
        return(0);
    }
    
    (*func)();
    dlclose(lib_ptr);
    return 0;
}
