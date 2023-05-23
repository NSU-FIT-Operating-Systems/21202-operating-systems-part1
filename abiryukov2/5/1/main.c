#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_variable = 10; // глобальная переменная

int main() {
    int local_variable = 20; // локальная переменная

    // i. создает и инициализирует переменную (можно две: локальную и глобальную)
    // ii. выводит ее (их) адрес(а) и содержимое
    printf("Global variable: address=%p, value=%d\n", &global_variable, global_variable);
    printf("Local variable: address=%p, value=%d\n", &local_variable, local_variable);

    // iii. выводит pid
    printf("PID: %d\n", getpid());

    // iv. порождает новый процесс (используйте fork(2))
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // v. в дочернем процессе выводит pid и parent pid
        printf("Child process: PID=%d, Parent PID=%d\n", getpid(), getppid());

        // vi. в дочернем процессе выводит адреса и содержимое переменных, созданных в пункте а
        printf("Child process: Global variable: address=%p, value=%d\n", &global_variable, global_variable);
        printf("Child process: Local variable: address=%p, value=%d\n", &local_variable, local_variable);

        // vii. в дочернем процессе изменяет содержимое переменных и выводит их значение
        global_variable = 30;
        local_variable = 40;
        printf("Child process: Modified values - Global variable: %d, Local variable: %d\n", global_variable, local_variable);

        // x. в дочернем процессе завершается с кодом “5” (exit(2))
        exit(5);
    } else {
        // viii. в родительском процессе выводит содержимое переменных
        printf("Parent process: Global variable: %d, Local variable: %d\n", global_variable, local_variable);

        // ix. в родительском процессе делает sleep(30)
        sleep(30);

        // xi. в родительском процессе дожидается завершения дочернего,
        // вычитывает код завершения и выводит причину завершения и код завершения если он есть.
        // В каком случае кода завершения не будет?
        int status;
        pid_t child_pid = wait(&status);

        if (WIFEXITED(status)) {
            printf("Child process terminated normally with exit status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process terminated abnormally.\n");
        }
    }

    return 0;
}
