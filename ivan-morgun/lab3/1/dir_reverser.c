#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_SIZE 3


void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void generateReversedPath(char *srcPath, char **reversedPath) {
    int srcPathLength = strlen(srcPath);
    *reversedPath = (char *) calloc(srcPathLength + 2, sizeof(char));

    for (int i = 0, j = 0; i <= srcPathLength; i++) {
        if (i == srcPathLength || srcPath[i] == '/') {
            for (int k = j; k < i; k++) {
                (*reversedPath)[k] = srcPath[j + i - k - 1];
            }
            (*reversedPath)[i] = '/';
            j = i + 1;
            mkdir(*reversedPath, 777);
        }
    }
}

void copy(char *src, char **dst) {
    *dst = (char *) calloc(strlen(src) + 1, sizeof(char));
    strcpy(*dst, src);
}

void reverseFile(char *srcFilePath, char *reversedFilePath) {
    int srcFD = open(srcFilePath, O_RDONLY);
    int reversedFD = open(reversedFilePath, O_CREAT | O_WRONLY);

    if (srcFD == -1 || reversedFD == -1) {
        perror("Error opening file");
        return;
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
        totalReadNumber += readNumber;
        if (shift == 0) {
            buffer[readNumber] = '\0';
        }
        reverseString(buffer);
        write(reversedFD, buffer, readNumber);
        toRead = BUFFER_SIZE;
    } while (shift != 0);

    close(srcFD);
    close(reversedFD);
    free(buffer);
}

void reverseDir(char *srcPath) {
    struct dirent *entry;

    DIR *dir = opendir(srcPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    char *reversedPath;
    generateReversedPath(srcPath, &reversedPath);
    mkdir(reversedPath, 777);
    char *srcFilePath = (char *) calloc(strlen(srcPath) + BUFFER_SIZE + 2, sizeof(char));
    char *reversedFilePath = (char *) calloc(strlen(reversedPath) + BUFFER_SIZE + 2, sizeof(char));
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *fileName;
            copy(entry->d_name, &fileName);

            sprintf(srcFilePath, "%s/%s\0", srcPath, fileName);

            reverseString(fileName);
            sprintf(reversedFilePath, "%s%s\0", reversedPath, fileName);

            reverseFile(srcFilePath, reversedFilePath);
        }
        else if (entry->d_type == DT_DIR) {
            fprintf(stdin, "Directory %s was detected and skipped\n", entry->d_name);
        }
    }

    closedir(dir);
    free(reversedPath);
    free(srcFilePath);
    free(reversedFilePath);
}




int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *srcPath = argv[1];
    reverseDir(srcPath);

    return 0;
}

