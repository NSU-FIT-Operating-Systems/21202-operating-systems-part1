#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

#define EXIT(...) {      \
    perror(__VA_ARGS__); \
    exit(EXIT_FAILURE);  \
}

#define PORT 8888
#define BUFFER_SIZE 1024
#define ERROR -1
#define TRUE 1

int main(int argc, char **argv ) {
    int clientSocket = 0;
    if ((clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR) {
        EXIT("Failed to create socket");
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter message:\n");
    
    char *buffer = (char *) malloc(BUFFER_SIZE * sizeof(char));
    if (buffer == NULL) {
        close(clientSocket);
        EXIT("Failed to allocate memory for buffer");
    }

    int readCount = 0;

    while (TRUE) {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == ERROR) {
            close(clientSocket);
            free(buffer);
            EXIT("Failed to send data");
        }

        readCount = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (readCount == ERROR) {
            close(clientSocket);
            free(buffer);
            EXIT("Failed to receive data");
        }
        buffer[readCount] = '\0';

        printf("Received: %s\n", buffer);
    }

    close(clientSocket);
    free(buffer);

    return EXIT_SUCCESS;
}
