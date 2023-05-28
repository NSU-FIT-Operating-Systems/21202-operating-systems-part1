#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

const char* exitMessage = "shutdown\n";

int main() {
    int s, client_address_size;
    unsigned int namelen;
    struct sockaddr_in client, server;
    char * buf = (char * ) malloc(1024 * sizeof(char));

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = 0;
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr * ) & server, sizeof(server)) < 0) {
        perror("bind()");
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

        printf("Ready to recieve msg\n");

        client_address_size = sizeof(client);
        readCount = recvfrom(s, buf, 1024, 0, (struct sockaddr * ) & client, & client_address_size);

        if (readCount == -1) {
            close(s);
            free(buf);
            perror("could not recieve msg");
            exit(4);
        }

        buf[readCount] = '\0';
        printf("Server recieved: %s\n\n", buf);

        if (sendto(s, buf, readCount, 0, (struct sockaddr * ) & client, client_address_size) < 0) {
            close(s);
            free(buf);
            exit(4);
        }
        
        if (strcmp(buf, exitMessage) == 0) {
            close(s);
            free(buf);
            return 0;
        }
    }
}
