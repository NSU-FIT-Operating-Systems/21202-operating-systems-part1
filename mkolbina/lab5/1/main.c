#include <stdio.h>
#include <sys/unistd.h>
#include <wait.h>
#include <stdlib.h>

int main() {
    int local_var;
    local_var = 5;

    printf("Parent PID:   %d\n", getpid());
    printf("Inited local var:   %p %d\n", &local_var, local_var);

    int pid = fork();

    if (pid == 0) {
        printf("\nI'm a child, my pid is %d\n", getpid());
        printf("I'm a child pid. My parent is %d\n", getppid());

        printf("\nLocal var before changes:    %p %d\n", &local_var, local_var);
        local_var = 10;
        printf("Local var after changes:    %p %d\n", &local_var, local_var);
        return 5;

    }

    sleep(10);

    printf("\nParent local_var:  %p %d\n", &local_var, local_var);

    int status;
    pid_t wait_pid = wait(&status);

    if (wait_pid == -1) {
        perror("wait");
        return EXIT_FAILURE;
    }

    if (WIFEXITED(status)) {
        printf("\nChild process terminated with code %d\n", WEXITSTATUS(status));
    } else {
        printf("Child process terminated incorrectly\n");
    }
	sleep(30);
    return 0;
}
