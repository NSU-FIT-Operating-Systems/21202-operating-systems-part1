#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define EQUAL 0
#define ERROR -1
#define STDOUT 1
#define DEC 10

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

void createDirectory(const char *name) {
    mode_t fullAccessRights = S_IRUSR | S_IWUSR | S_IXUSR | \
                              S_IRGRP | S_IWGRP | S_IXGRP | \
                              S_IROTH | S_IWOTH | S_IXOTH;
    if (mkdir(name, fullAccessRights) == ERROR) {
        EXIT("Impossible to create directory");
    }

}

void printDirectoryContent(const char *name) {
    DIR *directory = opendir(name);
    if (directory == NULL) {
        EXIT("Impossible to open directory");
    }

    struct dirent *entry;
    while ((entry = readdir(directory)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(directory);
}

void removeDirectory(const char *name) {
    if (rmdir(name) == ERROR) {
        EXIT("Impossible to remove directory");
    }
}

void createFile(const char *name) {
    int fd = open(name, O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
    if (fd == ERROR) {
        EXIT("Impossible to create file");
    }

    close(fd);
}

void printFileContent(const char *name) {
    int fd = open(name, O_RDONLY);
    if (fd == ERROR) {
        EXIT("Impossible to open file");
    }

    const size_t bufferSize = 1024;
    char *buffer = (char *) malloc(bufferSize * sizeof(char));
    if (buffer == NULL) {
        close(fd);
        EXIT("Impossible to allocate memory for buffer");
    }

    int readCount = 0;
    while (readCount = read(fd, buffer, bufferSize)) {
        int writeCount = write(STDOUT, buffer, readCount);
        if (readCount != writeCount) {
            free(buffer);
            close(fd);
            EXIT("Impossible to print file");
        }
    }

    free(buffer);
    close(fd);
}

void removeFile(const char *name) {
    if (unlink(name) == ERROR) {
        EXIT("Impossible to remove file");
    }
}

void createSymLink(const char *srcName, const char *dstName) {
    if (symlink(srcName, dstName) == ERROR) {
        EXIT("Impossible to create symbolic link");
    }
}

void printSymLinkContent(const char *name) {
    const size_t bufferSize = 1024;
    char *buffer = (char *) malloc(bufferSize * sizeof(char));
    if (buffer == NULL) {
        EXIT("Impossible to allocate memory for buffer");
    }

    int readCount = readlink(name, buffer, bufferSize);
    if (readCount == ERROR) {
        free(buffer);
        EXIT("Impossible to read symbolic link");
    }
    buffer[readCount++] = '\n';
    buffer[readCount] = '\0';

    int writeCount = write(STDOUT, buffer, readCount);

    free(buffer);

    if (readCount != writeCount) {
        EXIT("Impossible to print symbolic link");
    }
}

void printSymLinkFileContent(const char *name) {
    printFileContent(name);
}

void removeSymLink(const char *name) {
    removeFile(name);
}

void createHardLink(const char *srcName, const char *dstName) {
    if (link(srcName, dstName) == ERROR) {
        EXIT("Impossible to create hard link");
    }
}

void removeHardLink(const char *name) {
    removeFile(name);
}

void printFileStatistics(const char *name) {
    struct stat fileStatistics;
    if (stat(name, &fileStatistics) == ERROR) {
        EXIT("Impossible to get file statistics");
    }

    printf("File name:          %s\n", name);
    printf("Is symbolic link:   %s\n", S_ISLNK(fileStatistics.st_mode) ? "true" : "false");
    printf("File size:          %ld bytes\n", fileStatistics.st_size);
    printf("Hard links to file: %ld\n", fileStatistics.st_nlink);
    printf("File inode:         %ld\n", fileStatistics.st_ino);
    
    printf("File access mode:   ");
    printf(S_ISDIR(fileStatistics.st_mode) ? "d" : "-");
    printf(fileStatistics.st_mode & S_IRUSR ? "r" : "-");
    printf(fileStatistics.st_mode & S_IWUSR ? "w" : "-");
    printf(fileStatistics.st_mode & S_IXUSR ? "x" : "-");
    printf(fileStatistics.st_mode & S_IRGRP ? "r" : "-");
    printf(fileStatistics.st_mode & S_IWGRP ? "w" : "-");
    printf(fileStatistics.st_mode & S_IXGRP ? "x" : "-");
    printf(fileStatistics.st_mode & S_IROTH ? "r" : "-");
    printf(fileStatistics.st_mode & S_IWOTH ? "w" : "-");
    printf(fileStatistics.st_mode & S_IXOTH ? "x" : "-");
    printf("\n");


}

void changeFileAccessMode(const char *name, const mode_t newAccessMode) {
    if (chmod(name, newAccessMode) == ERROR) {
        EXIT("Impossible to change file access mode");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Invalid command line arguments\n");
        return EXIT_FAILURE;
    }

    const char *utilLetter = argv[1];

    if (argc == 3 && strcmp(utilLetter, "a") == EQUAL) {
        const char *name = argv[2];
        createDirectory(name);
    } else if (argc == 3 && strcmp(utilLetter, "b") == EQUAL) {
        const char *name = argv[2];
        printDirectoryContent(name);
    } else if (argc == 3 && strcmp(utilLetter, "c") == EQUAL) {
        const char *name = argv[2];
        removeDirectory(name);
    } else if (argc == 3 && strcmp(utilLetter, "d") == EQUAL) {
        const char *name = argv[2];
        createFile(name);
    } else if (argc == 3 && strcmp(utilLetter, "e") == EQUAL) {
        const char *name = argv[2];
        printFileContent(name);
    } else if (argc == 3 && strcmp(utilLetter, "f") == EQUAL) {
        const char *name = argv[2];
        removeFile(name);
    } else if (argc == 4 && strcmp(utilLetter, "g") == EQUAL) {
        const char *srcName = argv[2];
        const char *dstName = argv[3];
        createSymLink(srcName, dstName);
    } else if (argc == 3 && strcmp(utilLetter, "h") == EQUAL) {
        const char *name = argv[2];
        printSymLinkContent(name);
    } else if (argc == 3 && strcmp(utilLetter, "i") == EQUAL) {
        const char *name = argv[2];
        printSymLinkFileContent(name);
    } else if (argc == 3 && strcmp(utilLetter, "j") == EQUAL) {
        const char *name = argv[2];
        removeSymLink(name);
    } else if (argc == 4 && strcmp(utilLetter, "k") == EQUAL) {
        const char *srcName = argv[2];
        const char *dstName = argv[3];
        createHardLink(srcName, dstName);
    } else if (argc == 3 && strcmp(utilLetter, "l") == EQUAL) {
        const char *name = argv[2];
        removeHardLink(name);
    } else if (argc == 3 && strcmp(utilLetter, "m") == EQUAL) {
        const char *name = argv[2];
        printFileStatistics(name);
    } else if (argc == 4 && strcmp(utilLetter, "n") == EQUAL) {
        const char *name = argv[2];

        char **end = NULL;
        mode_t newAccessMode = (mode_t) strtol(argv[3], end, DEC);
        if (errno == ERANGE) {
            EXIT("Impossible to interpret new access mode");
        }

        changeFileAccessMode(name, newAccessMode);
    } else {
        printf("Invalid command line arguments\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
