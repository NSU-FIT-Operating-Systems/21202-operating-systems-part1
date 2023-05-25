#include <stdio.h>

int* unsafe_function() {
    int local_var = 10;
    return &local_var;
}

int main() {
    int* unsafe_ptr = unsafe_function();
    printf("\tpointer: %p, value: %d\n", unsafe_ptr, *unsafe_ptr);
    return 0;
}
