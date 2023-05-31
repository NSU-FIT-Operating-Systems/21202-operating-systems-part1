#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define PARENT_COLOR "\033[0;32m" //green
#define CHILD_COLOR "\033[0;34m" //blue
#define DEFALUT_COLOR "\033[0m" //default

#define ERROR (-1)
#define CHILD_PID 0

int global_var = 1;

int main() {
    int local_var = 2;
    printf("%s", PARENT_COLOR);
    printf("[PARENT]: parent PID:         %d\n", getpid());
    printf("[PARENT]: parent global_var:  %p %d\n", &global_var, global_var);
    printf("[PARENT]: parent local_var:   %p %d\n", &local_var, local_var);

    printf("[PARENT]: fork\n");
    pid_t pid = fork();
    if (pid == ERROR) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == CHILD_PID) {
        printf("%s",CHILD_COLOR);
        printf("[CHILD ]: child PID:          %d\n", getpid());
        printf("[CHILD ]: child PPID:         %d\n", getppid());

        printf("[CHILD ]: child global_var:   %p %d\n", &global_var, global_var);
        printf("[CHILD ]: child local_var:    %p %d\n", &local_var, local_var);

        global_var = 3;
        local_var = 4;
        printf("[CHILD ]: set global var to %d\n", global_var);
        printf("[CHILD ]: set local var to %d\n", local_var);
        printf("[CHILD ]: child global_var:   %p %d\n", &global_var, global_var);
        printf("[CHILD ]: child local_var:    %p %d\n", &local_var, local_var);

        return 5;
    }

    sleep(1);

    printf("%s", DEFALUT_COLOR);
    printf("%s",PARENT_COLOR);
    printf("[PARENT]: parent global_var:  %p %d\n", &global_var, global_var);
    printf("[PARENT]: parent local_var:   %p %d\n", &local_var, local_var);

    int status;
    pid_t wait_pid = wait(&status);

    if (wait_pid == ERROR) {
        perror("wait");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("[PARENT]: Child process terminated with code %d\n", WEXITSTATUS(status));
    } else {
        printf("[PARENT]: Child process terminated incorrectly\n");
    }

    return EXIT_SUCCESS;
}