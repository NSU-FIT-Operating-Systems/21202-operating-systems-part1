#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 5000
#define BUFFER_SIZE 1024

void handle_error(const char *msg, int fd) {
    perror(msg);
    close(fd);
    exit(1);
}

void config_address(struct sockaddr_in *server_address) {
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = inet_addr(SERVER_IP);
    server_address->sin_port = htons(PORT);
}

void handle_server(int fd, char *buffer) {
    ssize_t bytes_read, bytes_written;

    bytes_written = write(fd, buffer, strlen(buffer));
    if (bytes_written == -1) {
        handle_error("Error occured while sending data", fd);
    }

    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        handle_error("Error occured while receiving data", fd);
    }
}

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    config_address(&server_address);

    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        handle_error("Error occured while establishing connection", client_socket);
    }

    printf("Write message to send to UDP server (to exit from application type 'exit'):\n");
    while (1) {
        fgets(buffer, BUFFER_SIZE, stdin);

        if (strcmp(buffer, "exit\n") == 0) {
            break;
        }

        handle_server(client_socket, buffer);

        printf("Server response: %s\n", buffer);
    }

    close(client_socket);

    return 0;
}
