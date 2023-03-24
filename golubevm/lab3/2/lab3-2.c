#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define MKD 926766319
#define LSD 2431885709
#define RMD 3955155456
#define CRT 3551601661
#define FPRINT 1280254760
#define RMF 1570162098
#define SMLK 4000955435
#define SMLPRINT 66014203
#define HRDLK 1938404124


unsigned int HashFAQ6(const char * str);
int getId(char* str);
int checkArgsNumber(int argc, int requiered);
void printError(const char* msg);

int mkd(const char* filename);
int lsd(const char* filename);
int rmd(const char* filename);
int crt(const char* filename);
int fprint(const char* filename);

void print(char* str, unsigned int hash) {
	printf("#define %s %u\n", str, hash);
}

int main(int argc, char* argv[]) {
	if (checkArgsNumber(argc, 1) < 0) {
		fprintf(stderr, "Not enough arguments. Try to pass filepath\n");
		return -1;
	}
	fprint(argv[1]);

	return 0;
}

int mkd(const char* filename) {
	if (mkdir(filename, 0777) < 0) {
		perror("Cannot create dir");
		return -1;
	}
	return 0;
}

int lsd(const char* filename) {
	DIR* dir;
	dir = opendir(filename);
	if (!dir) {
		perror("Cannot open dir");
		closedir(dir);
		return -1;
	}
	struct dirent* entry; 
	while ((entry = readdir(dir)) != NULL) {
		printf("%s\n", entry->d_name);
	}
	closedir(dir);
}

int rmd(const char* filename) {
	if (rmdir(filename) < 0) {
		perror("Cannot delete directory");
		return -1;
	}
	return 0;
}

int crt(const char* filename) {
	int fd = open(filename, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fd < 0) {
		perror("Cannot create file");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int fprint(const char* filename) {
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("Cannot open file");
		close(fd);
		return -1;
	}
	const int SIZE = 256;
	char buf[SIZE];
	int rd;
	while (rd = read(fd, buf, SIZE)) {
		write(1, buf, rd);
	}
	close(fd);
	return 0;
}




int checkArgsNumber(int argc, int requiered) {
	if (argc - 1 < requiered)
		return -1;
	return 0;
}

unsigned int HashFAQ6(const char * str) {

    unsigned int hash = 0;

    for (; *str; str++)
    {
        hash += (unsigned char)(*str);
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

int getId(char* str) {
	size_t size = strlen(str);
	char * start_ptr = str;
	for (int i = size - 1; i >= 0; i--) {
		if (str[i] == '/') {
			start_ptr = &(str[i + 1]);
			break;
		}
	}
	unsigned int hash = HashFAQ6(start_ptr);
	if (hash == MKD)
		return 1;
	if (hash == LSD) 
		return 2;
	if (hash == RMD)
		return 3;
	return -1;
}

