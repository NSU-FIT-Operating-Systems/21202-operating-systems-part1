#include <stdio.h>
#include <time.h>
#include <unistd.h>

void print_local_vars() {
    int local_var = 10; // локальная переменная
    static int static_local_var = 20; // статическая локальная переменная
    const int const_local_var = 30; // константа в функции

    printf("\ti. локальная в функции: %p\n", (void*)&local_var);
    printf("\tii. статическая в функции: %p\n", (void*)&static_local_var);
    printf("\tiii. константа в функции: %p\n", (void*)&const_local_var);
}

int global_init_var = 100; // глобальная инициализированная переменная
int global_uninit_var;     // глобальная неинициализированная переменная
const int global_const_var = 200; // глобальная константа

int main() {
    print_local_vars();
    printf("\tiv. глобальная инициализированная: %p\n", (void*)&global_init_var);
    printf("\tv. глобальная неинициализированная: %p\n", (void*)&global_uninit_var);
    printf("\tvi. глобальная константа: %p\n", (void*)&global_const_var);
    sleep(10);
    return 0;
}
