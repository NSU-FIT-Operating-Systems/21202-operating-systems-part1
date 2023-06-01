#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        perror("no port");
        return 1;
    }
	
    int socket_fd;
    struct sockaddr_in srv;
	
    memset(&srv, 0, sizeof(srv));
    srv.sin_family = AF_INET;
    srv.sin_port = htons(atoi(argv[1]));
    srv.sin_addr.s_addr = INADDR_ANY;
	
    socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (socket_fd == -1) {
        perror("socket error");
        return 1;
    }
	
    if (bind(socket_fd, (struct sockaddr*)&srv, sizeof(srv)) == -1) {
        perror("bind error");
        return 1;
    }
	
    struct sockaddr_in cli;
    int cli_addr_len;
    char buf[1024];
    while (true) {
	for (int i = 0; i < 1024; ++i) buf[i] = '\0';
        int len = recvfrom(socket_fd, buf, sizeof(buf), 0, (struct sockaddr*)&cli, &cli_addr_len);
        if (len == -1) {
            perror("recvfrom error");
            continue;
        }
        printf("received: %s [from %s]\n", buf, inet_ntoa(cli.sin_addr));
        if (sendto(socket_fd, buf, len, 0, (struct sockaddr*)&cli, cli_addr_len) == -1) {
            perror("sendto error");
            continue;
        }
    }
	
    return 0;
}
