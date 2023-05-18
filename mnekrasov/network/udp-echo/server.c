#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

#define PORT 8888
#define TRUE 1
#define ERROR -1
#define BUFFER_SIZE 1024

int main(int argc, char **argv ) {
    int serverSocket = 0;
    if ((serverSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
        EXIT("socket() failed");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
        close(serverSocket);
        EXIT("Failed to bind socket");
    }

    printf("Waiting for data...\n");

    int clientSocket = 0;
    int addrLength = 0;
    int readCount = 0;

    struct sockaddr_in clientAddr;

    char *buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        close(serverSocket);
        EXIT("Failed to allocate memory for buffer");
    }

    while (TRUE) {
        addrLength = sizeof(clientAddr);

        readCount = recvfrom(serverSocket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddr, &addrLength);
        if (readCount == ERROR) {
            close(serverSocket);
            free(buffer);
            EXIT("Failed to receive data");
        }
        buffer[readCount] = '\0';

        printf("Received: %s\n", buffer);

        if (sendto(serverSocket, buffer, readCount, 0, (struct sockaddr *) &clientAddr, addrLength) == ERROR) {
            close(serverSocket);
            free(buffer);
            EXIT("Failed to send data");
        }
    }

    close(serverSocket);
    free(buffer);

    return EXIT_SUCCESS;
}
