#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int reverseFile(int entryFd, int outFd, const char *filename);
void reverse(const char *str, char *rev, size_t len);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "%s\n", "No path passed via program arguments");
    return -1;
  }

  int entryFd = open(argv[1], O_RDONLY | O_DIRECTORY);
  if (entryFd < 0) {
    perror("Cannot open entry dir");
    close(entryFd);
    return -1;
  }

  DIR *entryDir;
  entryDir = fdopendir(entryFd);

  size_t dirLen = strlen(argv[1]);
  char *tmp = (char *)malloc((dirLen + 1) * sizeof(char));
  tmp[dirLen] = '\0';
  reverse(argv[1], tmp, dirLen);
  if (mkdir(tmp, S_IRWXU | S_IRWXG | S_IRWXO) == -1) {
    close(entryFd);
    closedir(entryDir);
    free(tmp);
    fprintf(stderr, "%s\n", "Cannot create reverse dir");
    return -1;
  }
  free(tmp);
  int outFd = open(tmp, O_RDONLY | O_DIRECTORY);

  if (outFd < 0) {
    close(entryFd);
    closedir(entryDir);
    close(outFd);
    perror("Cannot open reverse dir");
    return -1;
  }

  struct dirent *next;
  while ((next = readdir(entryDir)) != NULL) {
    if (next->d_type == DT_REG) {
      int res = reverseFile(entryFd, outFd, next->d_name);
      if (res < 0) {
        close(entryFd);
        closedir(entryDir);
        return -1;
      }
    }
  }

  close(entryFd);
  closedir(entryDir);

  return 0;
}

void reverse(const char *str, char *rev, size_t len) {
  for (int i = 0; i < len; ++i) {
    rev[i] = str[len - 1 - i];
  }
}

int reverseFile(int entryFd, int outFd, const char *filename) {
  int entry = openat(entryFd, filename, O_RDONLY);
  if (entry < 0) {
    close(entry);
    perror("Cannot open entry file");
    return -1;
  }
  off_t size = lseek(entry, 0l, SEEK_END);
  off_t remainingToCopy = size;
  const size_t BUFFER_SIZE = 1024;
  char buf[BUFFER_SIZE];
  char rev[BUFFER_SIZE];

  reverse(filename, rev, strlen(filename));
  rev[strlen(filename)] = '\0';
  int out = openat(outFd, rev, O_CREAT | O_WRONLY, 0700);
  if (out < 0) {
    perror("Cannot create reverse file");
    close(entry);
    close(out);
    return -1;
  }

  while (remainingToCopy) {
    size_t count;
    if (remainingToCopy >= BUFFER_SIZE) {
      count = BUFFER_SIZE;
      remainingToCopy -= count;
    } else {
      count = remainingToCopy;
      remainingToCopy = 0;
    }
    lseek(entry, remainingToCopy, SEEK_SET);
    ssize_t r = read(entry, buf, count);
    if (r != count) {
      fprintf(stderr, "%s\n", "Cannot read from entry file");
      close(entry);
      close(out);
      return -1;
    }
    reverse(buf, rev, count);
    ssize_t w = write(out, rev, count);
    if (w != count) {
      fprintf(stderr, "%s\n", "Cannot write to reverse file");
      close(entry);
      close(out);
      return -1;
    }
  }

  close(entry);
  close(out);
}
