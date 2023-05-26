#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

#define ERROR -1
#define CHILD_PROCESS 0
#define IGNORE_ARG 0

#define EXIT(...) { \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE); \
}

void executeChild() {
    ptrace(PTRACE_TRACEME, IGNORE_ARG, IGNORE_ARG, IGNORE_ARG);

    execl("executable/exe", "executable/exe", "arg1", "arg2", "arg3", NULL);
}

void executeParent(pid_t childPid) {
    int childStatus;
    wait(&childStatus);

    ptrace(PTRACE_SETOPTIONS, childPid, IGNORE_ARG, PTRACE_O_EXITKILL);
    ptrace(PTRACE_SETOPTIONS, childPid, IGNORE_ARG, PTRACE_O_TRACESYSGOOD);

    while (!WIFEXITED(childStatus)) {
        ptrace(PTRACE_SYSCALL, childPid, IGNORE_ARG, IGNORE_ARG);
        wait(&childStatus);
    
        struct user_regs_struct regs;

        if (WIFSTOPPED(childStatus) && (WSTOPSIG(childStatus) & 0x80)) {
            ptrace(PTRACE_GETREGS, childPid, IGNORE_ARG, &regs);
            printf("Syscall number %llu (in %08llx)", regs.orig_rax, regs.rip);
            printf(" with args %llu, %llu, %llu, %llu, %llu, %llu", regs.rdi, regs.rsi, regs.rdx, regs.r10, regs.r8, regs.r9);

            ptrace(PTRACE_SYSCALL, childPid, IGNORE_ARG, IGNORE_ARG);
            wait(&childStatus);
            ptrace(PTRACE_GETREGS, childPid, IGNORE_ARG, &regs);
            printf(" returned %llu\n", regs.rax);
        }
    }
}

int main() {
    pid_t pid;
    if ((pid = fork()) == ERROR) {
        EXIT("fork() failed");
    }

    if (pid == CHILD_PROCESS) {
        executeChild();
    } else {
        executeParent(pid);
    }

    return EXIT_SUCCESS;
}
