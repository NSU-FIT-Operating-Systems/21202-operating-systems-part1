#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 1000


void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int generateReversedPath(char *srcPath, char **reversedPath) {
    int srcPathLength = strlen(srcPath);
    *reversedPath = (char *) calloc(srcPathLength + 2, sizeof(char));

    for (int i = 0, j = 0; i <= srcPathLength; i++) {
        if (i == srcPathLength || srcPath[i] == '/') {
            for (int k = j; k < i; k++) {
                (*reversedPath)[k] = srcPath[j + i - k - 1];
            }
            (*reversedPath)[i] = '/';
            j = i + 1;
            if (j == srcPathLength) {
                return 0;
            }
            if (mkdir(*reversedPath, 0755) < 0) {
                perror("Cannot create dirctory");
                return -1;
            }
        }
    }
}

void copy(char *src, char **dst) {
    *dst = (char *) calloc(strlen(src) + 1, sizeof(char));
    strcpy(*dst, src);
}

int clear(int df1, int df2, char* buffer) {
    free(buffer);
    int c1res = close(df1); 
    int c2res = close(df2); 
    if (c1res < 0 || c2res < 0) {
        perror("Cannot close file");
        return -1;
    }
}

int reverseFile(char *srcFilePath, char *reversedFilePath) {
    int srcFD = open(srcFilePath, O_RDONLY);
    mode_t mode = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
    int reversedFD = open(reversedFilePath, O_CREAT | O_WRONLY, mode);

    if (srcFD == -1 || reversedFD == -1) {
        perror("Cannot open file");
        return -1;
    }

    int fileLength = lseek(srcFD, 0, SEEK_END);

    char *buffer = (char *) malloc(BUFFER_SIZE);
    int toRead = fileLength % BUFFER_SIZE;
    int totalReadNumber = 0;
    int shift;
    do {
        shift = fileLength - toRead - totalReadNumber;
        lseek(srcFD, shift, SEEK_SET);
        int readNumber = read(srcFD, buffer, toRead);
        if (readNumber < 0) {
            perror("Cannot read from file");
            clear(srcFD, reversedFD, buffer);
            return -1;
        }
        totalReadNumber += readNumber;
        if (shift == 0) {
            buffer[readNumber] = '\0';
        }
        reverseString(buffer);
        if (write(reversedFD, buffer, readNumber) < 0) {
            perror("Cannot write to file");
            clear(srcFD, reversedFD, buffer);
            return -1;
        } 
        toRead = BUFFER_SIZE;
    } while (shift != 0);
    
    if (clear(srcFD, reversedFD, buffer) < 0) {
        return -1;
    }
    return 0;
}

int reverseDir(char *srcPath) {
    struct dirent *entry;

    DIR *dir = opendir(srcPath);
    if (dir == NULL) {
        perror("Cannot open directory");
        return -1;
    }

    char *reversedPath;
    if (generateReversedPath(srcPath, &reversedPath) < 0) {
        perror("Cannot create reversed path");
        return -1;
    }
    char *srcFilePath = (char *) calloc(strlen(srcPath) + BUFFER_SIZE + 2, sizeof(char));
    char *reversedFilePath = (char *) calloc(strlen(reversedPath) + BUFFER_SIZE + 2, sizeof(char));
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *fileName;
            copy(entry->d_name, &fileName);

            sprintf(srcFilePath, "%s/%s\0", srcPath, fileName);

            reverseString(fileName);
            sprintf(reversedFilePath, "%s%s\0", reversedPath, fileName);

            if (reverseFile(srcFilePath, reversedFilePath) < 0) {
                free(reversedPath);
                free(srcFilePath);
                free(reversedFilePath);
                return -1;
            }
        }
        else if (entry->d_type == DT_DIR) {
            fprintf(stdin, "Directory %s was detected and skipped\n", entry->d_name);
        }
    }

    free(reversedPath);
    free(srcFilePath);
    free(reversedFilePath);
    if (closedir(dir) < 0) {
        perror("Cannot close directory");
        return -1;
    }
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *srcPath = argv[1];
    if (reverseDir(srcPath) < 0) {
        perror("Cannot reverse directory");
        return -1;
    }

    return 0;
}
