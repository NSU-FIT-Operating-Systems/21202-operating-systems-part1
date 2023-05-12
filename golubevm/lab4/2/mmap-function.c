#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGES 10
#define SLEEP 15


void get_pid();
char *get_mapped_area();
void change_mapped_memory_protection_to_none(char *m_begin);
void try_to_read_from_protected_memory(char *mem);
void try_to_write_to_protected_memory(char *mem);
void sigsegv_handler(int signo);

int main(int argc, char const *argv[])
{
	get_pid();

	char *mapped_area = get_mapped_area();
	// sleep(SLEEP);

	change_mapped_memory_protection_to_none(mapped_area);
	// sleep(SLEEP);

	signal(SIGSEGV, sigsegv_handler);
	// try_to_read_from_protected_memory(mapped_area + 10);
	try_to_write_to_protected_memory(mapped_area + 5);

	return 0;
}

void get_pid() {
	printf("pid: %d\n", getpid());
	// sleep(SLEEP);
}

char *get_mapped_area() {
	char *ptr = mmap(NULL, PAGE_SIZE * PAGES, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	printf("mapped area: %p\n", ptr);
	return ptr;
}

void change_mapped_memory_protection_to_none(char *m_begin) {
	int err = mprotect(m_begin, PAGE_SIZE, PROT_NONE);
	if (err != 0) {
		perror(strerror(errno));
		exit(1);
	}
}

void try_to_read_from_protected_memory(char *mem) {
	char rd = *mem;
}

void try_to_write_to_protected_memory(char *mem) {
	*mem = 'a';
}

void sigsegv_handler(int signo) {
	printf("Segmenatation fault occured.\n");
	signal(signo, SIG_IGN);
}
