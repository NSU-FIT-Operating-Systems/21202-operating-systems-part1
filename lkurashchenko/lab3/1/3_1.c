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

long get_size(FILE *file_stream);

int read_file(char *buffer, size_t size, FILE *in_stream);

int write_file(const char *buffer, size_t size, FILE *in_stream);

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
        fprintf(stderr, "snprintf insufficient length");
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
            closedir(dir_stream);
            return ERROR;
        }

        dir_entry = readdir(dir_stream);
    }
    if (closedir(dir_stream)) {
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

long get_size(FILE *file_stream) {
    int result = SUCCESS;
    long begin_pos = 0;
    long size = 0;

    begin_pos = ftell(file_stream);
    if (begin_pos == ERROR)
        return ERROR;

    result = fseek(file_stream, 0, SEEK_END);
    if (result == ERROR)
        return ERROR;

    size = ftell(file_stream);
    if (size == ERROR)
        return ERROR;

    result = fseek(file_stream, begin_pos, SEEK_SET);
    if (result == ERROR)
        return ERROR;

    return size;
}

int read_file(char *buffer, size_t size, FILE *in_stream) {
    int result = SUCCESS;
    size_t read = 0;
    size_t total_read = 0;

    while (total_read < size) {
        read = fread(buffer + total_read, sizeof(char), size - total_read, in_stream);

        result = ferror(in_stream);
        if (read == 0 && result != SUCCESS)
            return ERROR;

        total_read += read;
    }

    return SUCCESS;
}

int write_file(const char *buffer, size_t size, FILE *in_stream) {
    int result = SUCCESS;
    size_t written = 0;
    size_t total_written = 0;

    while (total_written < size) {
        written = fwrite(buffer + total_written, sizeof(char), size - total_written, in_stream);

        result = ferror(in_stream);
        if (written == 0 && result != SUCCESS)
            return ERROR;

        total_written += written;
    }

    return SUCCESS;
}

int reverse_file(const char *file_path, const char *reverse_file_path) {
    int result = SUCCESS;
    size_t reversed = 0;
    size_t file_size = 0;
    char buffer[BUFFER_MAX + 1] = {};
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
        fclose(src_file_stream);
        return ERROR;
    }

    file_size = get_size(src_file_stream);
    if (file_size == ERROR) {
        perror(file_path);
        fclose(dest_file_stream);
        fclose(src_file_stream);
        return ERROR;
    }

    while (reversed != file_size) {
        long offset = max(0, file_size - reversed - BUFFER_MAX);
        size_t count = file_size - offset - reversed;

        result = fseek(src_file_stream, offset, SEEK_SET);
        if (result == ERROR) {
            perror(file_path);
            break;
        }

        memset(buffer, 0, BUFFER_MAX);
        result = read_file(buffer, count, src_file_stream);
        if (result == ERROR) {
            perror(file_path);
            break;
        }

        reverse_string(buffer, buffer, count);
        result = write_file(buffer, count, dest_file_stream);
        if (result == ERROR) {
            perror(reverse_file_path);
            break;
        }

        reversed += count;
    }

    fclose(dest_file_stream);
    fclose(src_file_stream);
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
