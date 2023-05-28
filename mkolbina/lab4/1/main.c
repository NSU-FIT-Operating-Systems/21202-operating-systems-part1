#include <stdio.h>
#include <sys/unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

int global_init = 1;
int global_uninit;
const int const_global_init = 2;

int *InitLocalVar() {
    int var = 5;
    printf("\nLocal initialized var:       %p, %d\n", &var, var);
	return &var;
}

void BufferInHeap() {
    char *local_buffer = malloc(100 * sizeof(char));
    strcpy(local_buffer, "Hello, World!");
    printf("\nLocal buffer:           %s\n", local_buffer);
    free(local_buffer);
    printf("Local buffer (after free):           %s\n", local_buffer);

    char *second_buffer = malloc(100 * sizeof(char));
    strcpy(second_buffer, "Hello, second World!");
    printf("\nSecond buffer:           %s\n", second_buffer);

//    second_buffer += 50;

    free(second_buffer);
    printf("Second buffer (after free):           %s\n", second_buffer);
}

void EnvVar() {
    char *env = getenv("NAME");
    printf("\nEnvironment var:   %s\n", env);

    setenv("NAME", "USER", 1);
    env = getenv("NAME");
    printf("Environment var:   %s\n", env);
}

int main() {
    const int const_init = 3;
    static int static_init = 4;
    int local_init = 5;

    printf("Pid: %d\n\n", getpid());

    printf("Global uninit var:     %p, %d\n", &global_uninit, global_uninit);
    printf("Global init var:       %p, %d\n", &global_init, global_init);
    printf("Const global init var:       %p, %d\n", &const_global_init, const_global_init);
    printf("Local init var:              %p, %d\n", &local_init, local_init);
    printf("Static init var:             %p, %d\n", &static_init, static_init);
    printf("Const init var:        %p, %d\n", &const_init, const_init);


   // printf("%p %d", &InitLocalVar, InitLocalVar());

    printf("Inited Var in main: %p\n", InitLocalVar());

    //printf("\nFunc InitLocalVar:    %p\n", &InitLocalVar);

    BufferInHeap();

    EnvVar();

    sleep(500);

    return 0;
}

