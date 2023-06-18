#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

#define PORT 8888
#define ERROR -1
#define TRUE 1
#define CHILD 0
#define BUFFER_SIZE 1024
#define STDOUT 1
#define MAX_QUEUE_LENGTH 0

void handleClient(int clientSocket) {
    char *buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        close(clientSocket);
        EXIT("Failed to allocate memory for buffer");
    }

    ssize_t readCount = 0;
    while ((readCount = read(clientSocket, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT, buffer, readCount);
        write(clientSocket, buffer, readCount);
    }

    close(clientSocket);
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == ERROR) {
        EXIT("socket() failed");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
        close(serverSocket);
        EXIT("bind() failed");
    }

    if (listen(serverSocket, MAX_QUEUE_LENGTH) == ERROR) {
        close(serverSocket);
        EXIT("listen() failed");
    }

    printf("Waiting for connections...\n");

    int clientSocket = 0;
    struct sockaddr_in clientAddr;
    pid_t pid = 0;
    socklen_t clientAddrLength = 0;

    while (TRUE) {
        clientAddrLength = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLength);
        if (clientSocket == ERROR) {
            close(serverSocket);
            EXIT("accept() failed");
        }

        pid = fork();
        if (pid == ERROR) {
            close(serverSocket);
            EXIT("fork() failed");
        } else if (pid == CHILD) {
            handleClient(clientSocket);
            close(serverSocket);
            break;
        } else {
            close(clientSocket);
        }
    }

    close(serverSocket);

    return EXIT_SUCCESS;
}
