#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/*
  how to use it:
  [command] [args]
  where command is command from the list below.

  To use this programm you need to compile it and create
  hard links to compiled file with proper name. Or you 
  can use Makefile to build all components.

  Commands:
  	mkd:
  		mkd dirname
  		Creates new directory.
  	lsd:
  		lsd dirname
  		Prints all files contained in quered directory.
  	rmd:
  		rmd dirname
  		Remove directory.
  	crt:
  		crt filename
  		Creates a regular file.
  	fprint:
  		fprint filename
  		Print all data contained in file to stdout. If you
  		apply this command to symlink it will show content of file
  		which path link stores.
  	rmf:
  		rmf filename
  		Unlink file. If there no more hardlinks, file will be deleted.
  	smlk:
  		smlk path
  		Creates symlink to file.
  	smlprint:
  		smlprint filename
  		Prints path stored in symlink.
  	hrdlink:
  		hrdlink filename
  		Creates hard link. To remove hardlink use rmf.
  	fstt:
  		fstt filename
  		Shows file statistics.
  	chmd:
  		chmd filename
  		Change current file permissions.
*/

int checkArgsNumber(int argc, int requiered);
char *getName(char* str);

int mkd(const char* filename);
int lsd(const char* filename);
int rmd(const char* filename);
int crt(const char* filename);
int fprint(const char* filename);
int rmf(const char* filename);
int smlk(const char* filename, const char* target);
int smlprint(const char* filename);
int hrdlk(const char* oldpath, const char* newpath);
int fstt(const char* filename);
int chmd(const char* filename, int mode);

int main(int argc, char* argv[]) {
	if (checkArgsNumber(argc, 1) < 0) {
		fprintf(stderr, "Not enough arguments. At least one requiered\n");
		return -1;
	}
	char* fun = getName(argv[0]);
	char* arg1 = argv[1];
	int res = 0;

	if (strcmp(fun, "mkd") == 0) {
		res = mkd(arg1);
	} else if (strcmp(fun, "lsd") == 0) {
		res = lsd(arg1);
	} else if (strcmp(fun, "rmd") == 0) {
		res = rmd(arg1);
	} else if (strcmp(fun, "crt") == 0) {
		res = crt(arg1);
	} else if (strcmp(fun, "fprint") == 0) {
		res = fprint(arg1);
	} else if (strcmp(fun, "rmf") == 0) {
		res = rmf(arg1);
	} else if (strcmp(fun, "smlk") == 0) {
		if (checkArgsNumber(argc, 2) < 0) {
			fprintf(stderr, "Not enough arguments. Two args requiered\n");
			return -1;
		}
		res = smlk(arg1, argv[2]);
	} else if (strcmp(fun, "smlprint") == 0) {
		res = smlprint(arg1);
	} else if (strcmp(fun, "hrdlk") == 0) {
		if (checkArgsNumber(argc, 2) < 0) {
			fprintf(stderr, "Not enough arguments. Two args requiered\n");
			return -1;
		}
		res = hrdlk(arg1, argv[2]);
	} else if (strcmp(fun, "fstt") == 0) {
		res = fstt(arg1);
	} else if (strcmp(fun, "chmd") == 0) {
		res = chmd(arg1, atoi(argv[2]));
	}

	return res;
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
		return -1;
	}
	struct dirent* entry; 
	while ((entry = readdir(dir)) != NULL) {
		printf("%s\n", entry->d_name);
	}
	closedir(dir);
	return 0;
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
		return -1;
	}
	close(fd);
	return 0;
}

int fprint(const char* filename) {
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		perror("Cannot open file");
		return -1;
	}
	const int SIZE = 256;
	char buf[SIZE];
	int rd;
	while (rd = read(fd, buf, SIZE)) {
		int wrt = write(1, buf, rd);
		if (rd != wrt) {
			return -1;
		}
	}
	close(fd);
	return 0;
}


int rmf(const char* filename) {
	if (unlink(filename) < 0) {
		perror("Cannot remove file");
		return -1;
	}
	return 0;
}

int smlk(const char* filename, const char* target) {
	if (symlink(filename, target) < 0) {
		perror("Cannot create symlink");
		return -1;
	}
	return 0;
}

int smlprint(const char* filename) {
	const int SIZE = 256;
	char buf[SIZE];
	int rd = readlink(filename, buf, SIZE);
	buf[rd] = '\0';
	if (rd < 0) {
		perror("Cannot read symlink");
		return -1;
	}

	int wrt = write(1,buf,rd);
	if (rd != wrt) {
		return -1;
	}
	return 0;
}

int hrdlk(const char* oldpath, const char* newpath) {
	if (link(oldpath, newpath) < 0) {
		perror("Cannot create hardlink");
		return -1;
	}
	return 0;
}

int fstt(const char* filename) {
	struct stat fileStat;
	if(stat(filename, &fileStat) < 0)    
	       return -1;

	   printf("Information for %s\n", filename);
	   printf("---------------------------\n");
	   printf("File Size: \t\t%ld bytes\n", fileStat.st_size);
	   printf("Number of Links: \t%ld\n", fileStat.st_nlink);
	   printf("File inode: \t\t%ld\n", fileStat.st_ino);

	   printf("File Permissions: \t");
	   printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
	   printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
	   printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
	   printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
	   printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
	   printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
	   printf("\n\n");

	   printf("The file %s a symbolic link\n", (S_ISLNK(fileStat.st_mode)) ? "is" : "is not");

	   return 0;
}

int chmd(const char* filename, int mode) {
	if (chmod(filename, mode) < 0) {
		perror("Cannot change permissions");
		return -1;
	}
	return 0;
}



int checkArgsNumber(int argc, int requiered) {
	if (argc - 1 < requiered)
		return -1;
	return 0;
}

char *getName(char* str) {
	size_t size = strlen(str);
	char * start_ptr = str;
	for (int i = size - 1; i >= 0; i--) {
		if (str[i] == '/') {
			start_ptr = &(str[i + 1]);
			break;
		}
	}
	return start_ptr;
}

