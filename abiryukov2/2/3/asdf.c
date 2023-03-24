#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <asm/unistd.h>
#include <syscall.h>
#include "SyscallNames.h"

#define __NR_syscalls 335

int main(int argc, char *argv[]) {
    pid_t child_pid;
    long orig_eax;
    int status;
    int insyscall[10000] = {0};

    if (argc < 2) {
        printf("Usage: %s program [args...]\n", argv[0]);
        return 1;
    }

    child_pid = fork();

/*
Команда fork в операционной системе Linux создает новый процесс,
который полностью идентичен родительскому процессу,
включая текущее состояние процесса и содержимое памяти.
Новый процесс называется дочерним,
а родительский процесс продолжает выполняться как обычно.

После вызова fork, процесс-родитель получает идентификатор процесса (PID) дочернего процесса,
а дочерний процесс получает значение 0.
Это позволяет процессу, который вызвал fork, определить, в каком контексте он работает:
как родительский или дочерний процесс.
*/

    if (child_pid == 0) {
        /* Это код дочернего процесса */
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);  // позволяем родителю отслеживать наши системные вызовы
        /*
        request=PTRACE_TRACEME - режим отладки
        pid=0 - применяем к себе
        */
        execvp(argv[1], &argv[1]);  // запуск указанной программы
        printf("Error executing program\n");  // Если execvp вернет ошибку, то происходит выход из процесса
        exit(1);
    } else {
        /* Это код родительского процесса */
        wait(&status);  // Родитель ждет, пока дочерний процесс не получит сигнал остановки
        while (WIFSTOPPED(status)) {  // "если процесс остановлен сигналом, который вызвал его приостановку"
            orig_eax = ptrace(PTRACE_PEEKUSER, child_pid, 8 * ORIG_RAX, NULL);
            // читаем из регистра rax номер системного вызова
            if (insyscall[orig_eax] == 0) {
                insyscall[orig_eax] = 1;
                printf("Вход в системный вызов %s\n", callname(orig_eax));
            } else {
                insyscall[orig_eax] = 0;
                printf("Выход из системного вызова %s\n", callname(orig_eax));
            }
            ptrace(PTRACE_SYSCALL, child_pid, NULL, NULL);  // запускаем выполнение до следующего сискола
            wait(&status);  // и снова ждем
        }
    }

    return 0;
}
