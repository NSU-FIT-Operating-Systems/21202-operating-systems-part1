#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PARENT 0
#define CHILD 1

void print_variable(const char *msg, const int *var);
void show_variables(const int *global_variable, const int *local_variable);
void show_status(int status);
void show_tag(int tag);

int global_variable = 111;

int main(int argc, char *argv[]) {
	int local_variable = 222;
	show_tag(PARENT);
	show_variables(&global_variable, &local_variable);
	printf("PID: %d\n", getpid());

	pid_t fork_pid = fork();

	if (fork_pid > 0) {
		show_tag(PARENT);
		show_variables(&global_variable, &local_variable);

		sleep(30);

		int status;
		int res = wait(&status);
		if (res < 0) {
			perror("Error catched while waiting for child process");
			exit(EXIT_FAILURE);
		}

		show_tag(PARENT);
		show_status(status);
	} else if (fork_pid == 0) {
		show_tag(CHILD);
		printf("PID: %d\n", getpid());
		printf("Parent PID: %d\n", getppid());

		show_variables(&global_variable, &local_variable);

		global_variable = 333;
		local_variable = 444;

		show_tag(CHILD);
		show_variables(&global_variable, &local_variable);

		exit(5);

	} else {
		perror("Error catched in fork");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void print_variable(const char *msg, const int *var) {
	printf("%s: %p:(%d)\n", msg, var, *var);
}

void show_variables(const int *global_variable, const int *local_variable) {
	print_variable("global var", global_variable);
	print_variable("local_variable", local_variable);
}

void show_status(int status) {
	if (WIFEXITED(status)) {
		printf("Child exited with status: %d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("Child terminated by signal: %d\n", WTERMSIG(status));
	} else if (WIFSTOPPED(status)) {
		printf("Child is stopped by signal: %d\n", WSTOPSIG(status));
	} else if (WIFCONTINUED(status)) {
		printf("Child was resumed by delivery of SIGCONT\n");
	}
}

void show_tag(int tag) {
	if (tag == CHILD) {
		printf("CHILD PROCESS\n");
	} else if (tag == PARENT) {
		printf("PARENT PROCESS\n");
	}
}

