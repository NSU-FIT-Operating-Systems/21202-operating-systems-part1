#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define PARENT 0
#define CHILD 1

void print_var_info(const char *msg, const int *var);
void show_vars(const int *global_var, const int *local_var);
void interpret_status(int status);
void print_tag(int tag);

int global_var = 100;

int main(int argc, char *argv[]) {
	int local_var = 200;
	print_tag(PARENT);
	show_vars(&global_var, &local_var);
	printf("pid: %d\n", getpid());

	pid_t fork_pid = fork();

	if (fork_pid > 0) {
		print_tag(PARENT);
		show_vars(&global_var, &local_var);

		sleep(30);

		int status;
		int res = wait(&status);
		if (res < 0) {
			perror("Error ocured while waiting for child process");
			exit(EXIT_FAILURE);
		}

		print_tag(PARENT);
		interpret_status(status);
	} else if (fork_pid == 0) {
		print_tag(CHILD);
		printf("pid: %d\n", getpid());
		printf("parent pid: %d\n", getppid());
		show_vars(&global_var, &local_var);


		global_var = 300;
		local_var = 400;
		print_tag(CHILD);
		show_vars(&global_var, &local_var);

		exit(5);

	} else {
		perror("Error occured at fork invocation");
		exit(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}

void print_var_info(const char *msg, const int *var) {
	printf("%s: %p:(%d)\n", msg, var, *var);
}

void show_vars(const int *global_var, const int *local_var) {
	print_var_info("global var", global_var);
	print_var_info("local_var", local_var);
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


