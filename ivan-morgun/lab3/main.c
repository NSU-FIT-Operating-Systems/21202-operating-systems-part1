#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1000

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void reverse_file(char *srcFilePath, char *dstFilePath) {
    FILE *srcFile = fopen(srcFilePath, "rb");
    FILE *dstFile = fopen(dstFilePath, "wb");
    if (srcFp == NULL || dstFp == NULL) {
        perror("Error opening file");
        return;
    }
    
    int fileSize = ftell();
    char *buffer = (char *) malloc(BUFFER_SIZE);
    size_t readNumber;
    do {
        readNumber = fread(buffer, 1, BUFFER_SIZE, srcFile);
        if (readNumber < BUFFER_SIZE) {
            buffer[readNumber] = '\0';
        }
        reverse_string(buffer);
        fseek(dstFile, 0, SEEK_SET);
        fwrite(buffer, 1, readNumber, dstFile);
    } while (readNumber == BUFFER_SIZE);

    fclose(srcFile);
    fclose(dstFile);
    free(buffer);
}

void build_path(char *result, char *prefix, char *name) {
    result = (char *) malloc(strlen(prefix) + strlen(name) + 2); 
    sprintf(result, "%s/%s", prefix, name);
}

void copy(char *dst, char *src) {
    dst = (char *) malloc(strlen(src) + 1);
    strcpy(dst, src);
}

void copy_directory(char *prefix, char *srcDirName) {
    struct dirent *entry;
    char *dstDirName;
    char *srcPath;
    char *dstPath;

    copy(dstDirName, srcDirName);
    reverse_string(dstDirName);
    build_path(srcPath, prefix, srcDirName); 
    DIR *dir = opendir(srcPath);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    
    build_path(dstPath, prefix, dstDirName);
    mkdir(dstPath, 777);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *srcFilePath;
            char *dstFileName;
            char *dstFilePath;
            build_path(srcFilePath, srcPath, entry->d_name);
            copy(dstFileName, entry->d_name);
            reverse_string(dstFileName);
            build_path(dstFilePath, dstPath, dstFileName);
            reverse_file(srcFilePath, dstFilePath);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char *srcSubdir = (char *) malloc(strlen(srcDir) + strlen(entry->d_name) + 2);
            sprintf(srcSubdir, "%s/%s", srcDir, entry->d_name);
            char *dstSubdir = (char *) malloc(strlen(dstDir) + strlen(entry->d_name) + 2);
            sprintf(dstSubdir, "%s/%s", dstDir, entry->d_name);
            reverse_string(dstSubdir);
            copy_directory(srcSubdir, dstSubdir);
            free(srcSubdir);
            free(dstSubdir);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *srcDir = argv[1];
    char *dstDir = (char *) malloc(strlen(srcDir));
    strcpy(dstDir, srcDir);
    reverse_string(dstDir);
    copy_directory(srcDir, dstDir);
    free(dstDir);
    return 0;
}