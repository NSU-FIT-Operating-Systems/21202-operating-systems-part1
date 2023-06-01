#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>

void swap(char* a, char* b) {
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

int GetDirNamePos(char* path, int pathLength) {
    int dirNamePos = 0;
    for (int i = pathLength - 1; i >= 0; --i) {
        if (path[i] == '/') {
            dirNamePos = i + 1;
            break;
        }
    }
    return dirNamePos;
}

char* GetPathReverseDir(char* path) {
    int pathLength = strlen(path);
    char* reverseDirName = (char*)malloc(pathLength);
    if (reverseDirName == NULL) {
        printf("Error: unable to malloc");
        exit(-1);
    }
    strcpy(reverseDirName, path);

    int dirNamePos = GetDirNamePos(path, pathLength);
    for (int i = dirNamePos; i < (pathLength + dirNamePos) / 2; ++i) {
        swap(&(reverseDirName[i]), &(reverseDirName[pathLength - 1 - i + dirNamePos]));
    }

    return reverseDirName;
}

int GetSeporatorPossition(char* str, int length, char seporator) {
    int pos = 0;
    for (; pos < length; ++pos) {
        if (str[pos] == seporator) break;
    }

    return pos;
}

char* GetReverseFileName(char* fileName, int lenFileName) {
    char* reverseFileName = (char*)malloc(lenFileName);
    if (reverseFileName == NULL) {
        printf("Error: unable to malloc");
        exit(-1);
    }
    memcpy(reverseFileName, fileName, lenFileName);

    int seporPos = GetSeporatorPossition(fileName, lenFileName, '.');
    for (int i = 0; i < seporPos / 2; ++i) {
        swap(&reverseFileName[i], &reverseFileName[seporPos - i - 1]);
    }

    return reverseFileName;
}

char* Concatenate(char* pathDir, int lenPathDir, char* fileName, int lenFileName) {
    int resultLength = lenPathDir + 1 + lenFileName;
    char* result = (char*)malloc(resultLength);
    if (result == NULL) {
        printf("Error: unable to malloc");
        exit(-1);
    }
    memcpy(result, pathDir, lenPathDir);
    result[lenPathDir] = '/';
    memcpy(result + lenPathDir + 1, fileName, lenFileName);

    return result;
}

int MakeReverseFile(char* pathOrigDir, char* pathReverseDir, char* fileName) {
    int lenFileName = 0;
    for (int i = 0; fileName[i] != '\0'; ++i) {
        lenFileName++;
    }

    FILE* origFile, * reverseFile;

    char* pathOrigFile = Concatenate(pathOrigDir, strlen(pathOrigDir), fileName, lenFileName);
    if ((origFile = fopen(pathOrigFile, "rb")) == NULL) {
        printf("Error: unable to open this file:\n%s\n", pathOrigFile);
        free(pathOrigFile);
        return 1;
    }

    char* reverseFileName = GetReverseFileName(fileName, lenFileName);
    char* pathReverseFile = Concatenate(pathReverseDir, strlen(pathReverseDir), reverseFileName, lenFileName);
    if ((reverseFile = fopen(pathReverseFile, "wb")) == NULL) {
        printf("Error: unable to open this file:\n%s\n", pathReverseFile);
        if (fclose(origFile) == EOF) {
            printf("Error: unable to close file:\n%s\n", pathOrigFile);
            free(pathOrigFile);
            free(pathReverseFile);
            return 1;
        }
        free(pathOrigFile);
        free(pathReverseFile);
        return 0;
    }

    fseek(origFile, -1, SEEK_END);
    int sizeOrigFile = ftell(origFile);
    for (int filePos = sizeOrigFile; filePos >= 0; --filePos) {
        fseek(origFile, filePos, SEEK_SET);
        fputc(fgetc(origFile), reverseFile);
    }

    if (fclose(origFile) == EOF) {
        printf("Error: unable to close file:\n%s\n", pathOrigFile);
        free(pathOrigFile);
        free(reverseFileName);
        free(pathReverseFile);
        return 1;
    }
    if (fclose(reverseFile) == EOF) {
        printf("Error: unable to close file:\n%s\n", pathReverseFile);
        free(pathOrigFile);
        free(reverseFileName);
        free(pathReverseFile);
        return 1;
    }
    free(pathOrigFile);
    free(reverseFileName);
    free(pathReverseFile);
    return 0;
}

int ReverseRegularFiles(char* pathOrigDir, char* pathReverseDir) {
    DIR* dp;
    if ((dp = opendir(pathOrigDir)) == NULL) {
        printf("Error: unable to open directory:\n%s\n", pathOrigDir);
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_REG) {
            if (MakeReverseFile(pathOrigDir, pathReverseDir, entry->d_name) == 1){
                printf("Error: unable to reverse file:\n%s\n", entry->d_name);
                return 1;
            }
        }
    }
    if (closedir(dp) == EOF){
        printf("Error: unable to close dir:\n%s\n", pathOrigDir);
        return 1;
    }
    return 0;
}

int MakeReverseDir(char* pathOrigDir) {
    struct stat dirstat;
    if (stat(pathOrigDir, &dirstat) == -1) {
        printf("Error: dirstat. Please, write ./main [DIRECTORY TO REVERSE]\n");
        return 1;
    }
    char* pathReverseDir = GetPathReverseDir(pathOrigDir);
    mkdir(pathReverseDir, dirstat.st_mode);
    if (ReverseRegularFiles(pathOrigDir, pathReverseDir) == 1) {
        printf("Error: unable to reverse files in:\n%s\n", pathOrigDir);
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    int ret = MakeReverseDir(argv[1]);
    if (ret == 1) {
        printf("Error: unable to reverse dir. Please, write ./main [DIRECTORY TO REVERSE]\n");
        return 1;
    }

    return 0;
}



