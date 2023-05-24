#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define TRUE 1

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

int globalNotInitedVar;
int globalInitedVar = 0;
const int globalConstVar = 0;

void printVarsAddresses() {
    int localNotInitedVar;
    int localInitedVar = 0;
    static int staticNotInitedVar;
    static int staticInitedVar = 0;
    const int constVar = 0;

    printf("localNotInitedVar: %p\n", &localNotInitedVar);
    printf("localInitedVar: %p\n", &localInitedVar);
    printf("staticNotInitedVar: %p\n", &staticNotInitedVar);
    printf("staticInitedVar: %p\n", &staticInitedVar);
    printf("constVar: %p\n\n", &constVar);

    printf("globalNotInitedVar: %p\n", &globalNotInitedVar);
    printf("globalInitedVar: %p\n", &globalInitedVar);
    printf("globalConstVar: %p\n\n", &globalConstVar);

}

// int *getLocalVarAddress() {
//     int localInitedVar = 0;
//     return &localInitedVar;
// }

void printBuffer() {
    const int bufferSize = 100;
    
    char *buffer = (char *) malloc(bufferSize);
    if (buffer == NULL) {
        EXIT("Impossible to allocate memory for buffer");
    }
    strcpy(buffer, "Hello, world!");
    printf("buffer:\n");
    for (int i = 0; i < bufferSize; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n\n");
    free(buffer);

    printf("buffer:\n");
    for (int i = 0; i < bufferSize; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n\n");

    buffer = (char *) malloc(bufferSize);
    if (buffer == NULL) {
        EXIT("Impossible to allocate memory for buffer");
    }
    strcpy(buffer, "Hello, world!");
    printf("buffer:\n");
    for (int i = 0; i < bufferSize; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n\n");

    buffer = buffer + bufferSize / 2;
    free(buffer);
    buffer = buffer - bufferSize / 2;
    printf("buffer:\n");
    for (int i = 0; i < bufferSize; ++i) {
        printf("%c", buffer[i]);
    }
    printf("\n\n");
}

void envVar() {
    // export ENV="Old value" 

    char *env = getenv("ENV");
    if (env == NULL) {
        printf("Impossible to get environment variable");
        exit(EXIT_FAILURE);
    }
    printf("Old ENV: %s\n", env);

    setenv("ENV", "New value", TRUE);
    env = getenv("ENV");
    if (env == NULL) {
        printf("Impossible to get environment variable");
        exit(EXIT_FAILURE);
    }
    printf("New ENV: %s\n\n", env);

    // echo $ENV
}

int main(int argc, char **argv) {
    printf("Process ID: %d\n\n", getpid());

    printVarsAddresses();
    // printf("localInitedVar: %p\n\n", getLocalVarAddress());
    // printBuffer();
    // envVar();

    sleep(100000);

    return EXIT_SUCCESS;
}
