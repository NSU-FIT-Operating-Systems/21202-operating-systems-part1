#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

int glob = 110;

int main() {
    int loc = 111;

    printf("global: %p (%d)\n", &glob, glob);
    printf("local: %p (%d)\n", &loc, loc);
    printf("pid: %d\n", getpid());
    
    pid_t child_pid = fork();

    if (child_pid == -1) {
        printf("Can't create child process\n");
        return 1;
    }
    else if (child_pid == 0) {
        printf("pid child: %d\n", getpid());
        printf("pid parent: %d\n", getppid());
        printf("global child: %p (%d)\n", &glob, glob);
        printf("local child: %p (%d)\n", &loc, loc);
        glob = 112;
        loc = 113;
        sleep(30);
        printf("global changed: %d\n", glob);
        printf("local changed: %d\n", loc);
        exit(5);
    }
    else {
	sleep(40);
	printf("global: %d\n", glob);
    	printf("local: %d\n", loc);
        int status;
        pid_t terminated_pid = wait(&status);
        if (WIFEXITED(status)) {
            printf("Child process %d finished with code %d\n", terminated_pid, WEXITSTATUS(status));
        }
        else {
            printf("Child process %d finished incorrect\n", terminated_pid);
        }
    }
    return 0;
}
