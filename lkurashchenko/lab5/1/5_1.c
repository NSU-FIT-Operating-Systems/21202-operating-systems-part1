#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define ERROR (-1)
#define CHILD_PID 0

int global_var = 1;

int main() {
    int local_var = 2;

    printf("parent PID:         %d\n", getpid());
    printf("parent global_var:  %p %d\n", &global_var, global_var);
    printf("parent local_var:   %p %d\n", &local_var, local_var);

    pid_t pid = fork();
    if (pid == ERROR) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == CHILD_PID) {
        printf("child PID:          %d\n", getpid());
        printf("child PPID:         %d\n", getppid());

        printf("child global_var:   %p %d\n", &global_var, global_var);
        printf("child local_var:    %p %d\n", &local_var, local_var);

        global_var = 3;
        local_var = 4;
        printf("child global_var:   %p %d\n", &global_var, global_var);
        printf("child local_var:    %p %d\n", &local_var, local_var);

        return 5;
    }

    sleep(1);

    printf("parent global_var:  %p %d\n", &global_var, global_var);
    printf("parent local_var:   %p %d\n", &local_var, local_var);

    int status;
    pid_t wait_pid = wait(&status);

    if (wait_pid == ERROR) {
        perror("wait");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("Child process terminated with code %d\n", WEXITSTATUS(status));
    } else {
        printf("Child process terminated incorrectly\n");
    }

    return EXIT_SUCCESS;
}