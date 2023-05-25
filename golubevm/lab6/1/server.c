#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_PORT 8888

int main() {
    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUF_SIZE];

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        perror("Error occured while creating socket");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error occured while binding socket");
        exit(1);
    }

    printf("UDP server is up. Waiting for clients...\n");

    while (1) {
        int receivedBytes = recvfrom(serverSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (receivedBytes < 0) {
            perror("Error occured while receiving data from client");
            close(serverSocket);
            exit(1);
        }

        if (sendto(serverSocket, buffer, receivedBytes, 0, (struct sockaddr *)&clientAddr, addrLen) < 0) {
            perror("Error occured while sending data to client");
            exit(1);
        }
    }

    close(serverSocket);

    return 0;
}