#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global_not_inited_var;
int global_inited_var = 1;
const int global_const_var = 2;

void heap_buffer();

int *init_local_var();

int main() {
    int local_var;
    static int static_var;
    const int local_const_var = 3;

    printf("pid: %d\n\n", getpid());

    printf("global not inited variable  pointer  %p,    value: %d\n", &global_not_inited_var, global_not_inited_var);
    printf("global initialized variable pointer  %p,    value %d\n", &global_inited_var, global_inited_var);
    printf("global const variable       pointer  %p,    value %d\n", &global_const_var, global_const_var);
    printf("static variable             pointer  %p,    value %d\n", &static_var, static_var);
    printf("local variable              pointer  %p,    value %d\n", &local_var, local_var);
    printf("local const variable        pointer  %p,    value %d\n", &local_const_var, local_const_var);

    int *func_pointer = init_local_var();
    printf("local function var in main  pointer  %p\n", func_pointer);

//    env
    char *env_var = "ENV_VAR";
    char *env_value;
    char *env_new_value = "Overwritten";
    env_value = getenv(env_var);
    printf("enviroment variable before:             %s\n", env_value);
    if (setenv(env_var, env_new_value, true) == (-1)) {
        perror("setenv fail");
        exit(EXIT_FAILURE);
    }
    env_value = getenv(env_var);
    printf("enviroment variable after:              %s\n", env_value);


    heap_buffer();

    sleep(100);

    return EXIT_SUCCESS;
}

int *init_local_var() {
    int local_variable = 4;
    printf("local function var  in func     pointer  %p\n", &local_variable);
    return &local_variable;
}

void heap_buffer() {
    int size = 100;
    char *local_buffer = malloc(size * sizeof(char));
    if (local_buffer == NULL) {
        perror("Couldnt malloc buffer");
        exit(EXIT_FAILURE);
    }
    strcpy(local_buffer, "Выделил память на куче");
    printf("local_buffer:\n          value: %s, pointer: %p,  middle: %p\n", local_buffer, &local_buffer,
           &local_buffer[50]);

    free(local_buffer);
    printf("local_buffer after free: value: %s, pointer: %p,  middle: %p\n", local_buffer, &local_buffer,
           &local_buffer[50]);

    local_buffer = malloc(size * sizeof(char));
    if (local_buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(local_buffer, "Выделил память на куче второй раз");
    printf("local_buffer:          value: %s, pointer: %p, middle: %p\n", local_buffer, &local_buffer,
           &local_buffer[50]);
    local_buffer += 50;
    free(local_buffer);
    printf("local_buffer:          value: %s, pointer: %p, middle: %p\n", local_buffer, &local_buffer,
           &local_buffer[50]);
}

