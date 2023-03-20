#include <sys/types.h>
#include <unistd.h>
// fork()
// exec()

#include <sys/ptrace.h>
// ptrace()

#include <sys/wait.h>
// wait()

#include <sys/user.h>
// struct user_regs_struct

#include <stdio.h>

#include "SyscallNames.h"

void executeChildProcess(char *args[]);
void traceChildProcess(pid_t child);
void initArgsList(char *args[], int argc, char *argv[]);

int main(int argc, char *argv[]) {
  pid_t child;
  char *argsList[argc];
  initArgsList(argsList, argc, argv);
  child = fork();
  if (child) {
    traceChildProcess(child);
  } else {
    executeChildProcess(argsList);
  }
  return 0;
}

void initArgsList(char *argsList[], int argc, char *argv[]) {
  int i = 0;
  while (i < argc - 1) {
    argsList[i] = argv[i + 1];
    i++;
  }
  argsList[i] = NULL;
}

void executeChildProcess(char *args[]) {
  ptrace(PTRACE_TRACEME, 0, 0, 0);
  execvp(args[0], args);
}

inline void traceChildProcess(pid_t child) {
  int status;

  while (waitpid(child, &status, 0) && !WIFEXITED(status)) {
    struct user_regs_struct regs;
    ptrace(PTRACE_GETREGS, child, NULL, &regs);
    fprintf(stderr, "Syscall: %s\n", callname(regs.orig_rax));
    ptrace(PTRACE_SYSCALL, child, NULL, NULL);
  }
}
