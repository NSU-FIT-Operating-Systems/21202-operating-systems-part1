#include <stdio.h>
#include <dlfcn.h>

void load_lib_and_run_func() {
    void *handle;  // opaque handle
    handle = dlopen("./libhello_shared.so", RTLD_LAZY);  // открываем библиотеку. lazy значит че-то делаем только тогда, когда надо
    if (handle == NULL) {
        printf("dlopen failed (1): %s\n", dlerror());
        return;
    }
    dlerror(); // предварительный вызов для очистки какой-либо ошибки, которая могла существовать

    void (*dyn_print_hello)();  // собственно наша функция
    dyn_print_hello = (void (*)()) dlsym(handle, "hello_from_shared_lib");  // ищет символ и возвращает его значение

    char *error = dlerror();  // используем dlerror, чтобы разрешить потенциальную неоднозначность с NULL
    if (error != NULL) {
        printf("dlopen failed (2): %s\n", error);
        dlclose(handle);
        return;
    }
    dyn_print_hello();  // run_func
    dlclose(handle);  //
}

int main() {
    load_lib_and_run_func();
    return 0;
}
