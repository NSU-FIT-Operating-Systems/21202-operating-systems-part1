#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
/*
  dir path format: absolute or relative path.
                   NOTE dir name cannot end with '/'.
  args format    : path_of_input_dir path_of_output_dir
*/

typedef enum {
  NOT_ENOUGH_ARGUMENTS = 1,
  CANNOT_OPEN_SPECIFIED_DIR,
  CANNOT_OPEN_OUTPUT_DIR,
  CANNOT_OPEN_REVERSE_DIR,
  CANNOT_CREATE_REVERSE_DIR,
  CANNOT_OPEN_FILE,
  CANNOT_CREATE_REVERSE_FILE,
  READ_ERROR,
  WRITE_ERROR
} errors_t;


typedef struct {
  int source_dir_fd;
  int dest_dir_fd;
  const char *filename;
} reverse_entry_t;


void get_reverse_dir_filepath(char *buf, char *filepath);
int reverse_file(reverse_entry_t *entry);
void reverse(const char *str, char *rev, size_t len);

void handle_close_fd(const int *fds, int size);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "%s\n", "No path passed via program arguments. Required two arguments: path/to/input/dir path/to/output/dir");
    return NOT_ENOUGH_ARGUMENTS;
  }

  int fds[3];

  fds[0] = open(argv[1], O_RDONLY | O_DIRECTORY);

  if (fds[0] < 0) {
    perror("Cannot open specified directory");
    return CANNOT_OPEN_SPECIFIED_DIR;
  }

  DIR *source_dir;
  source_dir = fdopendir(fds[0]);

  if (source_dir == NULL) {
    perror("Cannot open specified directory");
    handle_close_fd(fds, 1);
    return CANNOT_OPEN_SPECIFIED_DIR;
  }

  fds[1] = open(argv[2], O_RDONLY | O_DIRECTORY);

  if (fds[1] < 0) {
    perror("Cannot open output directory");
    handle_close_fd(fds, 1);
    closedir(source_dir);
    return CANNOT_OPEN_OUTPUT_DIR;
  }

  char reverse_dir_name[256];
  get_reverse_dir_filepath(reverse_dir_name, argv[1]);

  if (mkdirat(fds[1], reverse_dir_name, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
    perror("Cannot create reverse directory of specified directory");
    handle_close_fd(fds, 2);
    closedir(source_dir);
    return CANNOT_CREATE_REVERSE_DIR;
  }

  fds[2] = openat(fds[1], reverse_dir_name, O_RDONLY | O_DIRECTORY);

  if (fds[2] < 0) {
    perror("Reverse directory is created but cannot be opened");
    handle_close_fd(fds, 2);
    closedir(source_dir);
    return CANNOT_OPEN_REVERSE_DIR;
  }

  struct dirent *next;
  int program_result = 0;
  while ((next = readdir(source_dir)) != NULL) {
    if (next->d_type == DT_REG) {
      reverse_entry_t entry;
      entry.source_dir_fd = fds[0];
      entry.dest_dir_fd = fds[2];
      entry.filename = next->d_name;
      program_result = reverse_file(&entry);
      if (program_result) {
        break;
      }
    }
  }

  handle_close_fd(fds, 3);
  closedir(source_dir);

  return program_result;
}

void reverse(const char *str, char *rev, size_t len) {
  for (int i = 0; i < len; ++i) {
    rev[i] = str[len - 1 - i];
  }
}

int reverse_file(reverse_entry_t *entry) {
  int fd = openat(entry->source_dir_fd, entry->filename, O_RDONLY);
  if (fd < 0) {
    perror("Cannot open file at source directory");
    close(fd);
    return CANNOT_OPEN_FILE;
  }
  off_t size = lseek(fd, 0l, SEEK_END);
  off_t remaining_to_copy = size;
  const size_t BUFFER_SIZE = 1024;
  char buf[BUFFER_SIZE];
  char rev[BUFFER_SIZE];

  reverse(entry->filename, rev, strlen(entry->filename));
  rev[strlen(entry->filename)] = '\0';
  int out = openat(entry->dest_dir_fd, rev, O_CREAT | O_WRONLY, 0700);
  if (out < 0) {
    perror("Cannot create reverse file");
    close(fd);
    close(out);
    return CANNOT_CREATE_REVERSE_FILE;
  }

  while (remaining_to_copy) {
    size_t count;
    if (remaining_to_copy >= BUFFER_SIZE) {
      count = BUFFER_SIZE;
      remaining_to_copy -= count;
    } else {
      count = remaining_to_copy;
      remaining_to_copy = 0;
    }
    lseek(fd, remaining_to_copy, SEEK_SET);
    ssize_t r = read(fd, buf, count);
    if (r != count) {
      fprintf(stderr, "%s\n", "Cannot read from file");
      close(fd);
      close(out);
      return READ_ERROR;
    }
    reverse(buf, rev, count);
    ssize_t w = write(out, rev, count);
    if (w != count) {
      fprintf(stderr, "%s\n", "Cannot write to reverse file");
      close(fd);
      close(out);
      return WRITE_ERROR;
    }
  }

  close(fd);
  close(out);
  return 0;
}

void get_reverse_dir_filepath(char *buf, char *filepath) {
  size_t filepath_len = strlen(filepath);
  size_t rev_len = 0;
  char *begin = filepath;
  for (size_t i = filepath_len - 1; i >= 0; i--) {
    if (filepath[i] == '/') {
        begin = &filepath[i + 1];
        rev_len = filepath_len - i - 1;
        break;
    }
  }
  reverse(begin, buf, rev_len);
  buf[rev_len] = 0;
}

void handle_close_fd(const int *fds, int size) {
  for (int i = 0; i < size; ++i) {
    int res = close(fds[i]);
    if (res < 0) {
      perror("Error occured while closing file descriptor");
    }
  }
}
