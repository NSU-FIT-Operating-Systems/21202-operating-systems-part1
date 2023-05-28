#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    socklen_t addrLen = sizeof(serverAddr);
    char buffer[BUF_SIZE];

    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket < 0) {
        perror("Error occured while creating socket");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    printf("Write message to send to UDP server (to exit from application type 'exit'):\n");

    while (1) {
        fgets(buffer, BUF_SIZE, stdin);

        if (sendto(clientSocket, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, addrLen) < 0) {
            perror("Error occured while sending data to server");
            close(clientSocket);
            exit(1);
        }

        if (strcmp(buffer, "exit\n") == 0)
            break;

        int receivedBytes = recvfrom(clientSocket, buffer, BUF_SIZE, 0, (struct sockaddr *)&serverAddr, &addrLen);
        if (receivedBytes < 0) {
            perror("Error occured while receiving data from server");
            close(clientSocket);
            exit(1);
        }

        printf("Server response: %s", buffer);
    }

    close(clientSocket);
    return 0;
}
