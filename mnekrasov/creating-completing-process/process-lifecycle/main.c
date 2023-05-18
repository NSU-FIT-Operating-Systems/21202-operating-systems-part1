#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CHILD 0
#define ERROR -1

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

int globalVar = 1;

int main(int argc, char **argv) {
    int localVar = 2;

    printf("globalVar = %d (%p)\n", globalVar, &globalVar);
    printf("localVar = %d (%p)\n", localVar, &localVar);
    printf("\n");

    printf("pid: %d\n", getpid());
    printf("\n");

    pid_t pid = fork();
    
    if (pid == ERROR) {
        EXIT("fork() failed");
    } else {
        printf("Process forked, fork result: %d\n", pid);

        if (pid == CHILD) {
            printf("Child pid: %d\n", getpid());
            printf("Parent pid: %d\n", getppid());
            printf("\n");

            printf("In child process: globalVar = %d (%p)\n", globalVar, &globalVar);
            printf("                  localVar = %d (%p)\n", localVar, &localVar);
            printf("\n");

            globalVar = 10;
            localVar = 20;

            printf("In child process: new globalVar = %d\n", globalVar);
            printf("                  new localVar = %d\n", localVar);
            printf("\n");

            exit(5);
        } else {
            sleep(1);

            printf("In parent process: globalVar = %d\n", globalVar);
            printf("                   localVar = %d\n", localVar);
            printf("\n");

            sleep(30);

            int status = 0;
            pid_t childPid = wait(&status);
            if (childPid == ERROR) {
                EXIT("wait() failed");
            }

            if (WIFEXITED(status)) {
                printf("Child process status: %d\n", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                printf("Child process signal: %d\n", WTERMSIG(status));
            }

        }
    }

    return EXIT_SUCCESS;
}
