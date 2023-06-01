#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        perror("no ip or port");
        return 1;
    }

    int socket_fd;
    struct sockaddr_in srv;

    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(atoi(argv[2]));
    srv.sin_addr.s_addr = inet_addr(argv[1]);

    socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        perror("socket error");
        return 1;
    }

    char buf[1024];
    while(true) {
        printf("Enter message: ");
        fgets(buf, sizeof(buf), stdin);
        buf[strlen(buf) - 1] = '\0';
        if (!strcmp(buf, "exit")) break;
        int len = sendto(socket_fd, buf, strlen(buf), 0, (struct sockaddr*)&srv, sizeof(srv));
        if (len == -1) {
            perror("sendto error");
            continue;
        }
        if (recvfrom(socket_fd, buf, len, 0, NULL, NULL) == -1) {
            perror("recvfrom()");
            continue;
        }
        printf("recieved: %s\n", buf);
    }
    close(socket_fd);
    return 0;
}
