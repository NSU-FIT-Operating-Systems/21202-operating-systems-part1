#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PERM 0666
#define BUFF_SIZE 2

int main(int argc, char* argv[]) {
    //берем исходный каталог из ком строки
    char *src = argv[1];
    char dst[1024];
    //копируем исх кат в массив dst
    strcpy(dst, src);
    int len = 0;
    char temp = dst[0];
    while (temp != '\0') {
        ++len;
        temp = dst[len];
    }
    //переварачиваем строку в массиве dst
    for (int i = 0; i < len / 2; ++i) {
        temp = dst[i];
        dst[i] = dst[len - i - 1];
        dst[len - i - 1] = temp;
    }
    //создаем новый каталога с именем dst и правами доступа 0700
    mkdir(dst, 0700);
    //открываем поток ком строки для команды "ls"
    FILE *file;
    char path[1024], src_path[1024], dst_path[1024], cmd[1024] = "ls ", temp_path;
    int len_path;
    strcat(cmd, src);
    file = popen(cmd, "r");
    if (file == NULL) {
        perror("Start files copying error");
    }
    //читаем строки из потока ком строки
    while (fgets(path, sizeof(path), file) != NULL) {
        strcpy(src_path, src);
        strcpy(dst_path, dst);
        strcat(src_path, "/");
        strcat(dst_path, "/");
        len_path = 0;
        temp_path = path[0];

        while (temp_path != '\n') {
            ++len_path;
            temp_path = path[len_path];
        }
        path[len_path] = '\0';

        //формируем полной путь для исходного файла
        strcat(src_path, path);
        //проверяем тип файла
        int check_isdir = open(src_path, O_RDONLY);
        if (check_isdir == -1) {
            perror("Check type of src error");
            return 1;
        }
        struct stat buf_dir;
        if (fstat(check_isdir, &buf_dir)) {
            perror("fstat error");
            return 1;
        }

        // если файл не директория
        if (S_ISDIR(buf_dir.st_mode) == 0) {
            //переварачиваем
            for (int i = 0; i < len_path / 2; ++i) {
                temp_path = path[i];
                path[i] = path[len_path - i - 1];
                path[len_path - i - 1] = temp_path;
            }
            strcat(dst_path, path);
            int src_file = 0, dst_file = 0, n = -1;

            //открываем исх файл для чтения
            if ((src_file = open(src_path, O_RDONLY)) == -1) {
                perror("Open file error");
                return 1;
            }
            // создаем целевой файл с правами доступа PERM
            if ((dst_file = creat(dst_path, PERM)) == -1) {
                perror("Create file error");
                return 1;
            }
            //перемещаем указатель чтоб читать в конец файла
            lseek(src_file, n--, SEEK_END);
            //читаем из исх файла и пишем в целевой
            char buffer[BUFF_SIZE];
            do {
                if (read(src_file, buffer, BUFF_SIZE - 1) == 0) {
                    perror("Read file error");
                }
                buffer[1] = 0; // Добавление завершающего нуля в буфер
                if (write(dst_file, buffer, BUFF_SIZE - 1) == 0) {
                    perror("Write file error");
                }
            } while (lseek(src_file, n--, SEEK_END) > -1);
        }
    }

    return 0;
}
