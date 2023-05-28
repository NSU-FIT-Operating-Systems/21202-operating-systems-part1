#include <stdio.h>

#include <unistd.h>

#include <stdlib.h>

#include <sys/types.h>

#include <sys/wait.h>

int a = 0;

int main() {
    int b = 0;
    int status;
    pid_t w, pid;
    printf("&a = %p\n", & a);
    printf("&b = %p\n", & b);
    printf("pid is: %d\n", getpid());

    printf("\n\n\n");

    pid = fork();
    if (pid == 0) {
        printf("child pid is: %d\n", getpid());
        printf("parent pid is: %d\n", getppid());
        printf("From child:  &a = %p, a = %d\n", & a, a);
        printf("From child:  &b = %p, b = %d\n", & b, b);
        a = 10;
        b = 20;
        printf("From child:  &a = %p, a = %d\n", & a, a);
        printf("From child:  &b = %p, b = %d\n", & b, b);
        exit(5);
    }

    if (pid > 0) {
        sleep(2);
        printf("From parent:   &a = %p, a = %d\n", & a, a);
        printf("From parent:   &b = %p, b = %d\n", & b, b);
        sleep(30);
        w = waitpid(pid, & status, WUNTRACED | WCONTINUED);
        if (w == -1) {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status)) {
            printf("continued\n");
        }
    }

}
