#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/wait.h>

#define PORT 5000
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = read(client_socket, buffer, BUFFER_SIZE)) > 0) {
        write(client_socket, buffer, bytes_read);
    }

    close(client_socket);
    exit(0);
}

void config_server(struct sockaddr_in *server_address) {
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = htons(PORT);
}

void handle_error(const char *msg, int fd) {
    perror(msg);
    close(fd);
    exit(1);
}

void check_connections(int fd);

int open_connections = 0;

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error occured while creating socket");
        exit(1);
    }

    config_server(&server_address);

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        handle_error("Error occured while binding socket", server_socket);
    }

    if (listen(server_socket, 5) == -1) {
        handle_error("Error occured while binding socket", server_socket);
    }

    printf("TCP server is up. Waiting for connections...\n");

    while (1) {
        check_connections(server_socket);
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);
        open_connections++;
        if (client_socket == -1) {
            handle_error("Error occured while accepting connection", server_socket);
        }

        pid_t pid = fork();
        if (pid == -1) {
            close(client_socket);
            handle_error("Error occured while creating process for handling client", server_socket);
        } else if (pid == 0) {
            close(server_socket); 
            handle_client(client_socket);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);

    return 0;
}

void check_connections(int fd) {
     {
    for (int i = 0; i < open_connections; ++i)
    {
        pid_t pid;
        if ((pid = waitpid(-1, NULL, WNOHANG)) < 0) {
            handle_error("Error occured while waiting children", fd);
        }
        if (pid > 0) open_connections--;
    } 
}
}
