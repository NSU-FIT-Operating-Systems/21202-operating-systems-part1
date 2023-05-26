#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PARENT 0
#define CHILD 1

void interpret_status(int status);
void print_tag(int tag);

/*
	Здесь я показаываю, что происходит. Родительский процесс выводит информацию о себе,
	потом выполняет форк, снова выводит информацию о себе и засыпает, а затем завершается.
	В это время ребенок выводит информацию о себе, затем ждет пока родитель умрет, затем
	снова выводит информацию о себе, где должен быть уже другой pid родителя. 
	Наблюдать за этим удобно с помощью 'watch ps -x --forest'
*/

int main(int argc, char *argv[]) {
	print_tag(PARENT);
	printf("pid: %d\n", getpid());

	pid_t fork_pid = fork();

	if (fork_pid > 0) {
		print_tag(PARENT);

		sleep(5);

		exit(EXIT_FAILURE);
	} else if (fork_pid == 0) {
		print_tag(CHILD);
		printf("pid: %d\n", getpid());
		printf("parent pid: %d\n", getppid());
		sleep(10);
		printf("pid: %d\n", getpid());
		printf("parent pid: %d\n", getppid());
		sleep(10);
	} else {
		perror("Error occured at fork invocation");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void interpret_status(int status) {
	if (WIFEXITED(status)) {
		printf("Child exited with status: %d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("Child terminated by signal: %d\n", WTERMSIG(status));
		#ifdef WCOREDUMP
		printf("Child produced core dump: %d\n", WCOREDUMP(status));
		#endif
	} else if (WIFSTOPPED(status)) {
		printf("Child is stopped by signal: %d\n", WSTOPSIG(status));
	} else if (WIFCONTINUED(status)) {
		printf("Child was resumed by delivery of SIGCONT\n");
	}
}

void print_tag(int tag) {
	if (tag == CHILD) {
		printf("#Child process\n");
	} else if (tag == PARENT) {
		printf("#Parent process\n");
	}
}


