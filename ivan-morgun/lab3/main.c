#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

void reverse_string(char *str) {
    int len = strlen_s(str);
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
    char *buffer = (char *) malloc(file_size);
    fseek(fp, 0, SEEK_SET);
    fread(buffer, sizeof(char), file_size, fp);
    reverse_string(buffer);
    fseek(fp, 0, SEEK_SET);
    fwrite(buffer, sizeof(char), file_size, fp);
    fclose(fp);
    free(buffer);
}

void copy_directory(char *src_dir, char *dst_dir) {
    struct dirent *entry;
    DIR *dir = opendir(src_dir);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }
    mkdir(dst_dir, 0777);
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            char *src_file = (char *) malloc(strlen(src_dir) + strlen(entry->d_name) + 2);
            sprintf(src_file, "%s/%s", src_dir, entry->d_name);
            char *dst_file = (char *) malloc(strlen(dst_dir) + strlen(entry->d_name) + 2);
            sprintf(dst_file, "%s/%s", dst_dir, entry->d_name);
            reverse_string(dst_file);
            reverse_file(src_file);
            FILE *src_fp = fopen(src_file, "rb");
            FILE *dst_fp = fopen(dst_file, "wb");
            if (src_fp == NULL || dst_fp == NULL) {
                perror("Error opening file");
                return;
            }
            fseek(src_fp, 0, SEEK_END);
            int file_size = ftell(src_fp);
            char *buffer = (char *) malloc(file_size);
            fseek(src_fp, 0, SEEK_SET);
            fread(buffer, sizeof(char), file_size, src_fp);
            int i, j;
            char temp;
            for (i = 0, j = file_size - 1; i < j; i++, j--) {
                temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
            fwrite(buffer, sizeof(char), file_size, dst_fp);
            fclose(src_fp);
            fclose(dst_fp);
            free(src_file);
            free(dst_file);
            free(buffer);
        } else if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char *src_subdir = (char *) malloc(strlen(src_dir) + strlen(entry->d_name) + 2);
            sprintf(src_subdir, "%s/%s", src_dir, entry->d_name);
            char *dst_subdir = (char *) malloc(strlen(dst_dir) + strlen(entry->d_name) + 2);
            sprintf(dst_subdir, "%s/%s", dst_dir, entry->d_name);
            reverse_string(dst_subdir);
            copy_directory(src_subdir, dst_subdir);
            free(src_subdir);
            free(dst_subdir);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    char *src_dir = argv[1];
    char *dst_dir = (char *) malloc(strlen(src_dir) + 1);
    strcpy(dst_dir, src_dir);
    reverse_string(dst_dir);
    copy_directory(src_dir, dst_dir);
    free(dst_dir);
    return 0;
}