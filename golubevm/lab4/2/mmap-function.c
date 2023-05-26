#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <limits.h>

#define PAGE_SIZE 4096
#define PAGES 10
#define SLEEP 15


void get_pid();
char *get_mapped_area();
void change_mapped_memory_protection_to_none(char *m_begin);
void try_to_read_from_protected_memory(char *mem);
void try_to_write_to_protected_memory(char *mem);
void sigsegv_handler(int signo);
void unmap_pages(char *m_begin, int length);

int main(int argc, char const *argv[])
{
	get_pid();

	char *mapped_area = get_mapped_area();
	// sleep(SLEEP);

	change_mapped_memory_protection_to_none(mapped_area);
	// sleep(SLEEP);

	struct sigaction sa;
	sa.sa_handler = &sigsegv_handler;
        sigaction(SIGSEGV, &sa, NULL);
	// try_to_read_from_protected_memory(mapped_area + 10);
	// try_to_write_to_protected_memory(mapped_area + 5);
	unmap_pages(mapped_area + PAGE_SIZE * 4, PAGE_SIZE * 2);
	return 0;
}

void get_pid() {
	printf("pid: %d\n", getpid());
	// sleep(SLEEP);
}

char *get_mapped_area() {
	void *ptr = mmap(NULL, PAGE_SIZE * PAGES, PROT_EXEC|PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {
		perror("Error occured while mapping region");
		exit(EXIT_FAILURE);
	}
	printf("mapped area: %p\n", ptr);
	return ptr;
}

void change_mapped_memory_protection_to_none(char *m_begin) {
	int err = mprotect(m_begin, PAGE_SIZE, PROT_NONE);
	if (err != 0) {
		perror("Error occured while changing permission.");
		exit(EXIT_FAILURE);
	}
}

void try_to_read_from_protected_memory(char *mem) {
	char rd = *mem;
}

void try_to_write_to_protected_memory(char *mem) {
	*mem = 'a';
}

void sigsegv_handler(int signo) {
	const char* msg = "Segmentation fault got caught by custom handler\n";
	write(2, msg, strlen(msg));
	exit(EXIT_FAILURE);
}

void unmap_pages(char *m_begin, int length) {
	int res = munmap(m_begin, length);
	if (res != 0) {
		char msg[128];
		sprintf(msg, "Error occured while unmapping pages at: %p with length=%d", m_begin, length);
		perror(msg);
		exit(EXIT_FAILURE);
	}
}
