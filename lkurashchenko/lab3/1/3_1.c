#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SUCCESS 0
#define ERROR (-1)
#define BUFFER_MAX 256

long max(long a, long b);

void print_help(const char *program_name);

void reverse_string(char *dest, const char *src, int dest_length);

void get_file_name(const char *file_path, char *file_name);

int join_file_path(const char *working_dir, const char *file_name, char *file_path);

int reverse_from_argv(int argc, char **argv);

int reverse_dir(const char *dir_path, const char *new_dir_path);

int check_dir_entry(const struct dirent *dir_entry, const char *dest_working_dir, const char *src_working_dir);

int reverse_file(const char *file_path, const char *reverse_file_path);


long max(long a, long b) {
    return (a <= b) ? b : a;
}

void print_help(const char *program_name) {
    printf("Write in terminal: %s dir...\n", program_name);
}

void reverse_string(char *dest, const char *src, int dest_length) {
    char temp;
    char *begin = NULL;
    char *end = NULL;

    strncpy(dest, src, dest_length);
    begin = dest;
    end = dest + dest_length - 1;

    while (end > begin) {
        temp = *end;
        *end = *begin;
        *begin = temp;
        ++begin;
        --end;
    }
}

void get_file_name(const char *file_path, char *file_name) {
    char *last_slash = strrchr(file_path, '/');

    if (last_slash == NULL)
        strncpy(file_name, file_path, NAME_MAX);
    else
        strncpy(file_name, last_slash + 1, NAME_MAX);
}

int join_file_path(const char *working_dir, const char *file_name, char *file_path) {
    int result = SUCCESS;
    size_t file_name_length = strlen(file_name);
    size_t working_dir_length = strlen(working_dir);
    size_t file_path_length = file_name_length + working_dir_length;

    result = snprintf(file_path, PATH_MAX, "%s/%s", working_dir, file_name);
    if (result < 0) {
        perror("snprintf");
        return ERROR;
    }

    if (result != file_path_length + 1) {
        perror("snprintf insufficient length");
        return ERROR;
    }

    return SUCCESS;
}

int reverse_from_argv(int argc, char **argv) {
    int result = SUCCESS;
    char dir_name[NAME_MAX + 1] = {};
    char reverse_dir_name[NAME_MAX + 1] = {};
    char new_dir_path[PATH_MAX + 1] = {};

    for (int i = 1; i < argc; ++i) {
        get_file_name(argv[i], dir_name);
        reverse_string(reverse_dir_name, dir_name, strlen(dir_name));
        result = join_file_path(".", reverse_dir_name, new_dir_path);
        if (result == ERROR)
            return ERROR;
        result = reverse_dir(argv[i], new_dir_path);
        if (result == ERROR)
            return ERROR;
    }

    return SUCCESS;
}

int reverse_dir(const char *dir_path, const char *new_dir_path) {
    int result = SUCCESS;
    DIR *dir_stream = NULL;
    struct dirent *dir_entry = NULL;

    dir_stream = opendir(dir_path);
    if (dir_stream == NULL) {
        perror(dir_path);
        return ERROR;
    }

    result = mkdir(new_dir_path, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IXOTH);
    if (result == ERROR) {
        perror(new_dir_path);
        return ERROR;
    }

    dir_entry = readdir(dir_stream);
    while (dir_entry != NULL) {
        result = check_dir_entry(dir_entry, dir_path, new_dir_path);
        if (result == ERROR) {
            perror("check_dir_entry");
            if (closedir(dir_stream) == -1) {
                perror("closedir(dir_stream)");
            }
            return ERROR;
        }

        dir_entry = readdir(dir_stream);
    }
    if (closedir(dir_stream) == EOF) {
        return ERROR;
    }
    return SUCCESS;
}

int check_dir_entry(const struct dirent *dir_entry, const char *dest_working_dir, const char *src_working_dir) {
    int result = SUCCESS;
    char file_path[PATH_MAX + 1] = {};
    char reverse_file_name[NAME_MAX + 1] = {};
    char reverse_file_path[PATH_MAX + 1] = {};

    result = join_file_path(dest_working_dir, dir_entry->d_name, file_path);
    if (result == ERROR)
        return ERROR;

    reverse_string(reverse_file_name, dir_entry->d_name, strlen(dir_entry->d_name));

    result = join_file_path(src_working_dir, reverse_file_name, reverse_file_path);
    if (result == ERROR)
        return ERROR;

    if (dir_entry->d_type == DT_REG)
        result = reverse_file(file_path, reverse_file_path);
    else if (dir_entry->d_type == DT_DIR && strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
        result = reverse_dir(file_path, reverse_file_path);

    return result;
}

int reverse_file(const char *file_path, const char *reverse_file_path) {
    int result = SUCCESS;
    FILE *src_file_stream = NULL;
    FILE *dest_file_stream = NULL;

    src_file_stream = fopen(file_path, "r");
    if (src_file_stream == NULL) {
        perror(file_path);
        return ERROR;
    }

    dest_file_stream = fopen(reverse_file_path, "w");
    if (dest_file_stream == NULL) {
        perror(reverse_file_path);
        if (fclose(src_file_stream) == EOF) {
            perror("fclose");
        }
        return ERROR;
    }

    if (fseek(src_file_stream, -1, SEEK_END) != 0) {
        perror("fseek seek_end");
        return ERROR;
    }

    long size = ftell(src_file_stream);
    if (size == -1L) {
        perror("ftell");
    }
    for (long current_pos = size; current_pos >= 0; --current_pos) {
        if (fseek(src_file_stream, current_pos, SEEK_SET) != 0) {
            perror("fseek current_pos");
            return ERROR;
        }
        if (fputc(fgetc(src_file_stream), dest_file_stream) == EOF) {
            perror("fputc current_pos");
            return ERROR;
        }
    }

    if (fclose(dest_file_stream) == EOF) {
        perror("fclose");
        return ERROR;
    }
    if (fclose(src_file_stream) == EOF) {
        perror("fclose");
        return ERROR;
    }
    return result;
}


int main(int argc, char **argv) {
    int result = SUCCESS;

    if (argc < 2) {
        print_help(argv[0]);
        return EXIT_FAILURE;
    }

    result = reverse_from_argv(argc, argv);
    if (result == ERROR)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
