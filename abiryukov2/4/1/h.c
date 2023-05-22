#include <stdio.h>
#include <stdlib.h>

int main() {
    char* my_var = getenv("MY_VARIABLE");
    if (my_var)
        printf("MY_VARIABLE: %s\n", my_var);

    setenv("MY_VARIABLE", "Hello, World!", 1);

    my_var = getenv("MY_VARIABLE");
    if (my_var)
        printf("MY_VARIABLE: %s\n", my_var);

    return 0;
}
