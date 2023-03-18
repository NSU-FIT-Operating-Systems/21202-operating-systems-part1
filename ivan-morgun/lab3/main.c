#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void reverse_string(char *str) {
    int len = strlen(str);
    for (int i = 0, j = len - 1; i < j; i++, j--) {
        char temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

void reverse_file(char *filename) {
    FILE *fp = fopen(filename, "r+");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    fseek(fp, 0, SEEK_END);
    int fileSize = ftell(fp);
    char *buffer = (char *) malloc(fileSize);
    fseek(fp, 0, SEEK_SET);
    fread(buffer, sizeof(char), fileSize, fp);
    reverse_string(buffer);
    fseek(fp, 0, SEEK_SET);
    fwrite(buffer, sizeof(char), fileSize, fp);
    fclose(fp);
    free(buffer);
}

void copy_directory(char *srcDir, char *dstDir) {
    struct dirent *entry;
    DIR *dir = opendir(srcDir);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    mkdir(dstDir, 0777);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *srcFile = (char *) malloc(strlen(srcDir) + strlen(entry->d_name) + 2);
            sprintf(srcFile, "%s/%s", srcDir, entry->d_name);   
            char *dstFile = (char *) malloc(strlen(dstDir) + strlen(entry->d_name) + 2);
            sprintf(dstFile, "%s/%s", dstDir, entry->d_name);
            reverse_string(dstFile);
            reverse_file(srcFile);
            FILE *srcFp = fopen(srcFile, "rb");
            FILE *dstFp = fopen(dstFile, "wb");
            if (srcFp == NULL || dstFp == NULL) {
                perror("Error opening file");
                return;
            }
            fseek(srcFp, 0, SEEK_END);
            int fileSize = ftell(srcFp);
            char *buffer = (char *) malloc(fileSize);
            fseek(srcFp, 0, SEEK_SET);
            fread(buffer, sizeof(char), fileSize, srcFp);
            for (int i = 0, j = fileSize - 1; i < j; i++, j--) {
                char temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
            fwrite(buffer, sizeof(char), fileSize, dstFp);
            fclose(srcFp);
            fclose(dstFp);
            free(srcFile);
            free(dstFile);
            free(buffer);
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