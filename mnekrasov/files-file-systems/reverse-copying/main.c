#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define SUCCESS 0
#define ERROR -1
#define EQUAL 0

void reverseString(char *string) {
    int stringLen = strlen(string);

    for (int i = 0; i < stringLen / 2; ++i) {
        char temp = string[i];
        string[i] = string[stringLen - 1 - i];
        string[stringLen - 1 - i] = temp;
    }
}

void reverseFile(const char *srcFilePath, const char *dstFilePath) {
    FILE *srcFile = fopen(srcFilePath, "rb");
    if (!srcFile) {
        perror("Impossible to open source file for reading");
        return;
    }

    fseek(srcFile, 0, SEEK_END);
    long srcFileSize = ftell(srcFile);

    char *buffer = (char *) malloc(srcFileSize * sizeof(char));
    if (buffer == NULL) {
        perror("Impossible to allocate memory for buffer");
        fclose(srcFile);
        return;
    }
    fseek(srcFile, 0, SEEK_SET);
    fread(buffer, sizeof(char), srcFileSize, srcFile);
    fclose(srcFile);

    FILE *dstFile = fopen(dstFilePath, "wb");
    if (!dstFile) {
        perror("Impossible to open destination file for writing");
        free(buffer);
        return;
    }

    for (long i = srcFileSize - 1; i >= 0; --i) {
        fwrite(&buffer[i], sizeof(char), 1, dstFile);
    }

    fclose(dstFile);
    free(buffer);
}

void reverseDirectory(const char *srcDirPath, const char *dstDirPath) {
    DIR *srcDir = opendir(srcDirPath);
    if (!srcDir) {
        perror("Impossible to open source directory");
        return;
    }

    mode_t fullAccessRights = S_IRUSR | S_IWUSR | S_IXUSR | \
                              S_IRGRP | S_IWGRP | S_IXGRP | \
                              S_IROTH | S_IWOTH | S_IXOTH;
    if (mkdir(dstDirPath, fullAccessRights) == ERROR) {
        perror("Impossible to create destination directory");
        closedir(srcDir);
        return;
    }

    struct dirent *srcEntry;
    while ((srcEntry = readdir(srcDir)) != NULL) {
        if (srcEntry->d_type == DT_REG) {
            char *srcFileName = srcEntry->d_name;
            int srcFileNameLen = strlen(srcFileName);
            char *dstFileName = (char *) malloc((srcFileNameLen + 1) * sizeof(char));
            if (dstFileName == NULL) {
                perror("Impossible to allocate memory for dstFileName");
                closedir(srcDir);
                return;
            }
            strncpy(dstFileName, srcFileName, srcFileNameLen);
            reverseString(dstFileName);

            char *srcFilePath = (char *) malloc((strlen(srcDirPath) + strlen(srcFileName) + 2) * sizeof(char));
            if (srcFilePath == NULL) {
                perror("Impossible to allocate memory for srcFilePath");
                free(dstFileName);
                closedir(srcDir);
                return;
            }
            sprintf(srcFilePath, "%s/%s", srcDirPath, srcFileName);

            char *dstFilePath = (char *) malloc((strlen(dstDirPath) + strlen(dstFileName) + 2) * sizeof(char));
            if (dstFilePath == NULL) {
                perror("Impossible to allocate memory for dstFilePath");
                free(dstFileName);
                free(srcFilePath);
                closedir(srcDir);
                return;
            }
            sprintf(dstFilePath, "%s/%s", dstDirPath, dstFileName);

            reverseFile(srcFilePath, dstFilePath);

            free(dstFileName);
            free(srcFilePath);
            free(dstFilePath);

            if (errno != SUCCESS) {
                closedir(srcDir);
                return;
            }
        } else if (srcEntry->d_type == DT_DIR && strcmp(srcEntry->d_name, ".") != EQUAL && strcmp(srcEntry->d_name, "..") != EQUAL) {
            char *srcSubdirPath = (char *) malloc((strlen(srcDirPath) + strlen(srcEntry->d_name) + 2) * sizeof(char));
            if (srcSubdirPath == NULL) {
                perror("Impssible to allocate memory for srcSubdirPath");
                closedir(srcDir);
                return;
            }
            sprintf(srcSubdirPath, "%s/%s", srcDirPath, srcEntry->d_name);

            char *dstSubdirName = (char *) malloc((strlen(srcEntry->d_name + 1)) * sizeof(char));
            if (dstSubdirName == NULL) {
                perror("Impossible to allocate memory for dstSubdirName");
                free(srcSubdirPath);
                closedir(srcDir);
                return;
            }
            strncpy(dstSubdirName, srcEntry->d_name, strlen(srcEntry->d_name));
            reverseString(dstSubdirName);

            char *dstSubdirPath = (char *) malloc((strlen(dstDirPath) + strlen(dstSubdirName) + 2) * sizeof(char));
            if (dstSubdirPath == NULL) {
                perror("Impossible to allocate memory for dstSubdirPath");
                free(srcSubdirPath);
                free(dstSubdirName);
                closedir(srcDir);
                return;
            }
            sprintf(dstSubdirPath, "%s/%s", dstDirPath, dstSubdirName);

            reverseDirectory(srcSubdirPath, dstSubdirPath);

            free(srcSubdirPath);
            free(dstSubdirName);
            free(dstSubdirPath);
        }
    }

    closedir(srcDir);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s path/to/directory\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *srcDirPath = argv[1];
    int srcDirPathLen = strlen(srcDirPath);

    char *dstDirName = (char *) malloc((srcDirPathLen + 1) * sizeof(char));
    if (dstDirName == NULL) {
        perror("Impossible to allocate memory for dstDirName");
        return EXIT_FAILURE;
    }
    strncpy(dstDirName, srcDirPath, srcDirPathLen);
    reverseString(dstDirName);

    char *dstDirPath = (char *) malloc((srcDirPathLen + 1) * sizeof(char));
    if (dstDirPath == NULL) {
        perror("Impossible to allocate memory for dstDirPath");
        free(dstDirName);
        return EXIT_FAILURE;
    }
    sprintf(dstDirPath, "./%s", dstDirName);

    reverseDirectory(srcDirPath, dstDirPath);

    free(dstDirName);
    free(dstDirPath);

    return errno == SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;
}
