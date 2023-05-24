#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

#define ERROR -1
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main(int argc, char **argv) {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == ERROR) {
        EXIT("socket() failed");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
        close(clientSocket);
        EXIT("connect() failed");
    }

    char *buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        close(clientSocket);
        EXIT("Failed to allocate memory for buffer");
    }

    printf("Enter message: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    write(clientSocket, buffer, strlen(buffer));

    memset(buffer, 0, sizeof(buffer));

    if (read(clientSocket, buffer, BUFFER_SIZE) == ERROR) {
        free(buffer);
        close(clientSocket);
        EXIT("read() failed");
    }

    printf("Message from server: %s\n", buffer);

    free(buffer);
    close(clientSocket);

    return EXIT_SUCCESS;
}
