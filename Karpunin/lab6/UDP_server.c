#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int s;
    socklen_t namelen, client_address_size;
    struct sockaddr_in client, server;
    char * buf = (char * ) malloc(1024 * sizeof(char));

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Could not create socket");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = 0;
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr * ) & server, sizeof(server)) < 0) {
        perror("Could not bind socket");
        exit(2);
    }

    namelen = sizeof(server);
    if (getsockname(s, (struct sockaddr * ) & server, & namelen) < 0) {
        perror("getsockname()");
        exit(3);
    }

    int readCount = 0;

    printf("Port assigned is %d\n", ntohs(server.sin_port));

    while (1) {

        printf("Ready to receive msg\n");

        client_address_size = sizeof(client);
        readCount = recvfrom(s, buf, 1024, 0, (struct sockaddr * ) & client, & client_address_size);

        if (readCount == -1) {
            perror("could not receive msg");
            if (close(s) < 0) {
                perror("Could not close socket file\n");
            }
            free(buf); 
            exit(4);
        }

        buf[readCount] = '\0';
        printf("Server received: %s\n\n", buf);

        if (sendto(s, buf, readCount, 0, (struct sockaddr * ) & client, client_address_size) < 0) {
            perror("Could not send the message\n");
            if (close(s) < 0) {
                perror("Could not close socket file\n");   
            }   
            free(buf);
            exit(4);
        }
    }
}
