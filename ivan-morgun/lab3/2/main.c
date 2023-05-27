#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 1000


/*
Commands:
    mkd:
  		Usage: mkd <dir_name>
  		Analog: mkdir
  	lsd:
  		Usage: lsd <dir_name>
  		Analog: ls
  	rmd:
  		Usage: rmd <dir_name>
  		Analog: rm -r
  	tch:
  		Usage: tch <file_name>
  		Analog: touch
  	ctf:
  		Usage: ctf <file_name>
  		Analog: cat
  	rmf:
  		Usage: rmf <file_name>
  		Analog: rm
  	mksmlk:
  		Usage: mksmlk <file_path> <symbolic_link>
  		Analog: ln -s
  	ctsmlk:
  		Usage: ctsmlk <symbolic_link>
  		Analog: cat
    rmsmlk:
        Usage: rmsmlk <symbolic_link>
        Analog: rm
  	mkhrdlk:
  		Usage: mkhrdlk <file_path> <hard_link>
  		Analog: ln
    rmhrdlk:
        Usage: rmhrdlk <hard_link>
        Analog: rm
  	lslf:
  		Usage: lslf <file_name>
  		Analog: ls -l
  	chmd:
  		Usage: chmd <file_name>
  		Analog: chmod
*/

int mkd(const char* dirname);
int lsd(const char* dirname);
int rmd(const char* dirname);

int tch(const char* filename);
int ctf(const char* filename);
int rmf(const char* filename);

int mksmlk(const char* path, const char* linkName);
int ctsmlk(const char* linkName);

int mkhrdlk(const char* path, const char* linkName);

int lslf(const char* filename);
int chmd(const char* filename, int mode);

int checkArgsNumber(int argsNumber, int expectedArgsNumber);
int closeFile(int fileDesciptor);
char* getNameFromAbsPath(char* path);


int main(int argc, char *argv[]) {
    
    if (checkArgsNumber(argc, 1) < 0) {
		fprintf(stderr, "Not enough arguments\n");
		return -1;
	}

    printf("%s\n", argv[0]);
	char* functionName = getNameFromAbsPath(argv[0]);
	printf("%s\n", argv[0]);
	printf("%s\n", functionName);
	int result = 0;

	if (!strcmp(functionName, "mkd")) {
		result = mkd(argv[1]);
	} 
	else if (!strcmp(functionName, "lsd")) {
		result = lsd(argv[1]);
	}
	else if (!strcmp(functionName, "rmd")) {
		result = rmd(argv[1]);
	}
	else if (!strcmp(functionName, "tch")) {
		result = tch(argv[1]);
	}
	else if (!strcmp(functionName, "ctf")) {
		result = ctf(argv[1]);
	}
	else if (!strcmp(functionName, "rmf")) {
		result = rmf(argv[1]);
	}
	else if (!strcmp(functionName, "mksmlk")) {
		if (checkArgsNumber(argc, 2) < 0) {
			fprintf(stderr, "Not enough arguments\n");
			return -1;
		}
		result = mksmlk(argv[1], argv[2]);
	}
	else if (!strcmp(functionName, "ctsmlk")) {
		result = ctsmlk(argv[1]);
	}
	else if (!strcmp(functionName, "mkhrdlk")) {
		if (checkArgsNumber(argc, 2) < 0) {
			fprintf(stderr, "Not enough arguments\n");
			return -1;
		}
		result = mkhrdlk(argv[1], argv[2]);
	}
	else if (!strcmp(functionName, "lslf")) {
		result = lslf(argv[1]);
	}
	else if (!strcmp(functionName, "chmd")) {
		if (checkArgsNumber(argc, 2) < 0) {
			fprintf(stderr, "Not enough arguments\n");
			return -1;
		}
		result = chmd(argv[1], atoi(argv[2]));
	}

	return result;
}

int checkArgsNumber(int argsNumber, int expectedArgsNumber) {
    return argsNumber - 1 - expectedArgsNumber;
}

int closeFile(int fileDescriptor) {
	int result = close(fileDescriptor); 
	if (result < 0) {
		perror("Cannot close file");
	}
	return result;
}

char* getNameFromAbsPath(char* path) {
	for (int i = strlen(path) - 1; i >= 0; i--) {
		if (path[i] == '/') {
			return &(path[i + 1]);
		}
	}
	return path;
}

int mkd(const char* dirname) {
	int result = mkdir(dirname, 0777);
	if (result < 0) {
		perror("Cannot create directory");
	}
	return result;
}

int lsd(const char* dirname) {
	DIR* dir;
	dir = opendir(dirname);
	if (!dir) {
		perror("Cannot open directory");
		return -1;
	}
	struct dirent* entry; 
	while ((entry = readdir(dir)) != NULL) {
		char* typeTitle = "Unknown type";
		switch (entry->d_type) {
			case DT_REG:
				typeTitle = "Regular file";
				break;
			case DT_DIR:
				typeTitle = "Directory";
				break;
			case DT_FIFO:
				typeTitle = "FIFO Special File";
				break;
			case DT_SOCK:
				typeTitle = "Local-domain socket";
				break;
			case DT_CHR:
				typeTitle = "Character device";
				break;
			case DT_BLK:
				typeTitle = "Block device";
				break; 
			default:
				break;
		}
		printf("%s (%s)\n", entry->d_name, typeTitle);
	}
	if (closedir(dir) < 0) {
		perror("Cannot close directory");
		return -1;
	}
	return 0;
}

int rmd(const char* dirname) {
	int result = rmdir(dirname);
	if (result < 0) {
		perror("Cannot remove directory");
	}
	return result;
}

int tch(const char* filename) {
	int fd = open(filename, O_CREAT, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
	if (fd < 0) {
		perror("Cannot create file");
		return -1;
	}
	return closeFile(fd);
}

int ctf(const char* filename) {
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("Cannot open file");
		return -1;
	}
	char buffer[BUFFER_SIZE];
	int readNumber;
	while ((readNumber = read(fd, buffer, BUFFER_SIZE))) {
		int writtenNumber = write(STDOUT_FILENO, buffer, readNumber);
		if (writtenNumber != readNumber) {
			return closeFile(fd);
		}
	}
	return closeFile(fd);
}

int rmf(const char* filename) {
	int result = unlink(filename);
	if (result < 0) {
		perror("Cannot remove file");
	}
	return result;
}

int mksmlk(const char* path, const char* linkName) {
	int result = symlink(path, linkName);
	if (result < 0) {
		perror("Cannot create symbolic link");
	}
	return result;
}

int ctsmlk(const char* linkName) {
	char originalFilename[BUFFER_SIZE];
	int readNumber = readlink(linkName, originalFilename, BUFFER_SIZE);
	if (readNumber < 0) {
		perror("Cannot read symbolic link");
		return -1;
	}
	originalFilename[readNumber] = '\0';
	return ctf(originalFilename);
}

int mkhrdlk(const char* path, const char* linkName) {
	int result = link(path, linkName);
	if (result < 0) {
		perror("Cannot create hard link");
	}
	return result;
}

int lslf(const char* filename) {
	struct stat fileStat;
	int result = stat(filename, &fileStat);
	if (result < 0) {
		perror("Cannot get file stats");
		return result;
	}

	printf("%s stats:\n", filename);
	printf("Size: %lld bytes\n", fileStat.st_size);
	printf("Permissions: ");
	if (S_ISDIR(fileStat.st_mode))
		printf("d");
	else if (S_ISLNK(fileStat.st_mode))
		printf("l");
	else 
		printf("-");
	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf("\nLinks number: %hu\n", fileStat.st_nlink);

	return result;
}

int chmd(const char* filename, int mode) {
	int result = chmod(filename, mode);
	if (result < 0) {
		perror("Cannot change permissions");
	}
	return result;
}
