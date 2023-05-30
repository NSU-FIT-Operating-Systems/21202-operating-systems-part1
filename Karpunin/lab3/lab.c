#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void reverseString(char * string, int size) {
    char temp;
    size_t i = 0;
    size_t j = size - 1;

    while (i < j) {
        temp = string[i];
        string[i] = string[j];
        string[j] = temp;
        i++;
        j--;
    }
}

void getNewPath(char * path) {
    char currentChar = '0';
    
    size_t pathIterator = 0;
    size_t fileNamePosition = 0;
    
    while (currentChar != '\0') {
        currentChar = path[pathIterator++];
        if (currentChar == '/') {
            fileNamePosition = pathIterator;
        }
    }
    
    char name[PATH_MAX];
    strcpy(name, & path[fileNamePosition]);
    reverseString(name, strlen(name));
    strcpy( & path[fileNamePosition], name);
}

size_t getByteAmount(int fileDesc) {
    size_t byteAmount = 0;
    char temp;

    int readFlag;

    while ((readFlag = read(fileDesc, & temp, 1)) == 1) {
        i++;
    }

    if (readFlag < 0) {
        return -1;
    }

    if (lseek(fileDesc, 0, SEEK_SET) == -1) {
        perror("Could not set offset");
        return -1;
    }
    return byteAmount;
}

int createNewReversedFile(char * fileName, char * path, char * startPath) {
    
    char originalFilePath[strlen(startPath) + strlen(fileName) + 2];
    char newFilePath[strlen(path) + strlen(fileName) + 2];

    strcpy(originalFilePath, startPath);
    strcpy(originalFilePath + strlen(startPath), "/");
    strcpy(originalFilePath + strlen(startPath) + 1, fileName);

    reverseString(fileName, strlen(fileName));

    strcpy(newFilePath, path);
    strcpy(newFilePath + strlen(path), "/");
    strcpy(newFilePath + strlen(path) + 1, fileName);

    int fileDesc1 = open(originalFilePath, O_RDONLY);

    int fileDesc2 = open(newFilePath, O_RDWR | O_CREAT, S_IRWXU);

    if (fileDesc1 == -1 && fileDesc2 == -1) {
        perror("Could not open folders\n");
        return -1;
    }

    if (fileDesc2 == -1) {
        if (close(fileDesc1) < 0) {
            perror("Could not close original file\n");
        }
        perror("Could not open new file\n");
        return -1;
    }

    if (fileDesc1 == -1) {
        if (close(fileDesc2) < 0) {
            perror("Could not close new file\n");
        }
        perror("Could not open original file\n");
        return -1;
    }

    char temp;
    size_t i = 0;

    int byteAmount = getByteAmount(fileDesc1);
    if (byteAmount == -1) {
        int fileDescFlag1 = close(fileDesc1);
        int fileDescFlag2 = close(fileDesc2);

        if (fileDescFlag1 < 0 && fileDescFlag2 < 0) {
            printf("Could not close both files\n");
            return -1;
        }

        if (fileDescFlag1 < 0) {
            printf("Could not close original file\n");
            return -1;
        }

        if (fileDescFlag2 < 0) {
            printf("Could not close new file\n");
            return -1;
        }
    }

    if (lseek(fileDesc1, -1, SEEK_END) == -1) {
        perror("Could not set offset\n");
        return -1;
    }

    while (i++ != byteAmount) {
        if (read(fileDesc1, & temp, 1) == -1) {
            perror("Could not read files");
            int fileDescFlag1 = close(fileDesc1);
            int fileDescFlag2 = close(fileDesc2);

            if (fileDescFlag1 < 0 && fileDescFlag2 < 0) {
                printf("Could not close both files\n");
                return -1;
            }

            if (fileDescFlag1 < 0) {
                printf("Could not close original file\n");
                return -1;
            }

            if (fileDescFlag2 < 0) {
                printf("Could not close new file\n");
                return -1;
            }
            return -1;
        }
        
        
        if (lseek(fileDesc1, -2, SEEK_CUR) == -1) {
            perror("Could not set offset");
            return -1;
        }
        
        
        if (write(fileDesc2, & temp, 1) == -1) {
            perror("Could not write into files");
            int fileDescFlag1 = close(fileDesc1);
            int fileDescFlag2 = close(fileDesc2);

            if (fileDescFlag1 < 0 && fileDescFlag2 < 0) {
                printf("Could not close both files\n");
                return -1;
            }

            if (fileDescFlag1 < 0) {
                printf("Could not close original file\n");
                return -1;
            }

            if (fileDescFlag2 < 0) {
                printf("Could not close new file\n");
                return -1;
            }
            return -1;
        }
    }

    int flag = 0;
    if (close(fileDesc1) == -1) {
        perror("Could not close original folder\n");
        flag = 1;
    }
    if (close(fileDesc2) == -1) {
        perror("Could not close new folder\n");
        flag = 1;
    }
    if (flag != 0) {
        return -1;
    }
    return 0;
}

int main() {
    char path[PATH_MAX];
    printf("Folder path is: ");
    if (scanf("%s", path) <= 0) {
        perror("Could not read path\n");
        return EXIT_FAILURE;
    }

    char startPath[PATH_MAX];
    strcpy(startPath, path);

    DIR * directory = opendir(path);
    if (directory) {
        closedir(directory);
    } else if (ENOENT == errno) {
        perror("Directory does not exist:");
        printf("\n");
        return EXIT_FAILURE;
    } else {
        perror("Failed to open dir:");
        printf("\n");
        return EXIT_FAILURE;
    }

    getNewPath(path);

    if (mkdir(path, 0700) != 0) {
        perror("Could not create new folder\n");
        return EXIT_FAILURE;
    }

    DIR * d;
    struct dirent * dir;
    d = opendir(startPath);

    if (d == NULL) {
        perror("Could not open directory\n");
        return EXIT_FAILURE;
    }
    while ((dir = readdir(d)) != NULL) {
        if (dir -> d_type != DT_DIR) {
            if (createNewReversedFile(dir -> d_name, path, startPath) == -1) {
                return EXIT_FAILURE;
            }
        }
    }

    if (closedir(d) != 0) {
        perror("Could not close directory\n");
        return EXIT_FAILURE;
    }

    printf("Done!");
    return 0;
}
