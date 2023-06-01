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
    int server_sock;
    char buffer[BUFFER_SIZE] = {};
    struct sockaddr_in server_sock_addr;
    struct sockaddr_in client_sock_addr;
    socklen_t sock_addr_length = sizeof(struct sockaddr_in);

    // Create server socket
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = SERVER_PORT;
    server_sock_addr.sin_addr.s_addr = inet_addr(SERVER_IP);


    server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_sock == ERROR) {
        perror("socket");
        if (close(server_sock) == ERROR) {
            perror("close(server_sock)");
        }
        return EXIT_FAILURE;
    }

    ret = bind(server_sock, (struct sockaddr *) &server_sock_addr, sock_addr_length);
    if (ret == ERROR) {
        perror("bind");
        if (close(server_sock) == ERROR) {
            perror("close(server_sock)");
        }
        return EXIT_FAILURE;
    }


    // Start communication
    printf("Server is receiving messages on %s\n", SERVER_IP);
    printf("Server port is %d\n", SERVER_PORT);
    printf("To disconnect from server and stop it send 'DISCONNECT' command\n");
    printf("Waiting for message...\n");
    while (true) {
        ret = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock_addr,
                       &sock_addr_length);
        if (ret == ERROR) {
            perror("recvfrom");
            continue;
        }
        printf("Message received from %s:%d\n", inet_ntoa(client_sock_addr.sin_addr),
               client_sock_addr.sin_port);
        printf("Message content: %s\n\n", buffer);
        if (strncmp(buffer, CLIENT_DISCONNECT, 10) == 0) {
            printf("Client disconnected\n");
            printf("Server stops communication\n");
            break;
        }
        ret = sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_sock_addr,
                     sock_addr_length);
        if (ret == ERROR) {
            perror("sendto");
            continue;
        }
    }

    if (close(server_sock) == ERROR) {
        perror("close(server_sock)");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}