#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

char* strrev(char* str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

void reverse_copy(const char* src_dir) {
    // создаем целевой каталог
    char dst_dir[strlen(src_dir) + 1];
    strcpy(dst_dir, src_dir);
    strrev(dst_dir);
    mkdir(dst_dir, 0777);  // 0777 - полный доступ

    // открываем исходный каталог
    DIR* src_d = opendir(src_dir);
    if (src_d == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    // читаем и копируем файлы
    struct dirent* dir_entry;
    while ((dir_entry = readdir(src_d)) != NULL) {
        // пропускаем каталоги "." и ".."
        if (strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0) {
            continue;
        }

        // формируем пути к исходному и целевому файлам
        char src_file[strlen(src_dir) + strlen(dir_entry->d_name) + 2];
        sprintf(src_file, "%s/%s", src_dir, dir_entry->d_name);
        char dst_file[strlen(dst_dir) + strlen(dir_entry->d_name) + 2];
        sprintf(dst_file, "%s/%s", dst_dir, strrev(dir_entry->d_name));

        // получаем информацию о файле
        struct stat src_stat;
        if (stat(src_file, &src_stat) == -1) {
            perror("stat");
            exit(EXIT_FAILURE);
        }

        if (S_ISREG(src_stat.st_mode)) {  // ну, это файл, да?
            FILE* src_f = fopen(src_file, "rb");
            if (src_f == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            FILE* dst_f = fopen(dst_file, "wb");
            if (dst_f == NULL) {
                perror("fopen");
                exit(EXIT_FAILURE);
            }
            char buffer[src_stat.st_size];
            size_t read_size = fread(buffer, 1, src_stat.st_size, src_f);
            if (read_size != src_stat.st_size) {
                perror("fread");
                exit(EXIT_FAILURE);
            }
            for (int i = src_stat.st_size - 1; i >= 0; i--) {  // ну и переписваем файл, вот
                fputc(buffer[i], dst_f);
            }
            fclose(src_f);
            fclose(dst_f);
        }
    }

    closedir(src_d);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <src_dir>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    reverse_copy(argv[1]);

    return 0;
}
