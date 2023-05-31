#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define CLIENT_DISCONNECT "DISCONNECT"
#define ERROR           (-1)
#define BUFFER_SIZE     1024
#define SERVER_PORT     5000

const char *SERVER_IP = "127.0.0.1";

int main(int argc, char **argv) {
    int ret;
    int client_sock;
    char buffer[BUFFER_SIZE] = {};
    socklen_t sock_addr_length = sizeof(struct sockaddr_in);
    struct sockaddr_in server_sock_addr;

    // Create client socket
    client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_sock == ERROR) {
        perror("socket");
        if (close(client_sock) == ERROR){
            perror("close(client_sock)");
        }
        return EXIT_FAILURE;
    }

    // Initialize server address
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = SERVER_PORT;
    ret = inet_pton(AF_INET, SERVER_IP, &(server_sock_addr.sin_addr));
    if (ret == ERROR) {
        perror("inet_pton");
        if (close(client_sock) == ERROR){
            perror("close(client_sock)");
        }
        return EXIT_FAILURE;
    }
    printf("Write 'DISCONNECT' in order to disconnect from server\n");
    while (true) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }
        ret = sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &server_sock_addr,
                     sock_addr_length);
        if (ret == ERROR) {
            perror("sendto");
            continue;
        }
        if (strncmp(buffer, CLIENT_DISCONNECT,10) == 0){
            printf("Client disconnected\n");
            break;
        }
        ret = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_sock_addr,
                       &sock_addr_length);
        if (ret == ERROR) {
            perror("recvfrom");
            continue;
        }
        printf("Message received: %s\n\n", buffer);
    }

    if (close(client_sock) == ERROR) {
        perror("close(client_sock)");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}