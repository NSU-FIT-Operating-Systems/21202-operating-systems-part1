#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int s;
    unsigned short port;
    struct sockaddr_in server;
    char * buf = (char * ) malloc(1024 * sizeof(char));

    if (argc != 3) {
        printf("Usage: %s <host address> <port> \n", argv[0]);
        exit(1);
    }
    port = htons(atoi(argv[2]));

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = port;
    server.sin_addr.s_addr = inet_addr(argv[1]);

    int readCount = 0;

    while (1) {

        printf("Your message: \n");

        fgets(buf, 1024, stdin);

        if (sendto(s, buf, strlen(buf), 0,
                (struct sockaddr * ) & server, sizeof(server)) < 0) {
            perror("sendto()");
            exit(2);
        }

        readCount = recvfrom(s, buf, 1024, 0, NULL, NULL);

        if (readCount == -1) {
            close(s);
            free(buf);
            exit(4);
        }

        buf[readCount] = '\0';
        printf("Client recieved: %s\n", buf);
    }
    close(s);
    free(buf);
}
