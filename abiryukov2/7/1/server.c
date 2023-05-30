#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr);
    char buffer[MAX_BUFFER_SIZE];

    // Создание UDP сокета
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Ошибка при создании сокета");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Привязка сокета к адресу сервера
    if (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(1);
    }

    printf("UDP сервер запущен и ожидает сообщения от клиентов...\n");

    while (1) {
        // Получение данных от клиента
        int numBytesReceived = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr*)&clientAddr, &addrLen);
        if (numBytesReceived < 0) {
            perror("Ошибка при приеме данных");
            exit(1);
        }

        // Отправка данных обратно клиенту
        int numBytesSent = sendto(sockfd, buffer, numBytesReceived, 0, (struct sockaddr*)&clientAddr, addrLen);
        if (numBytesSent < 0) {
            perror("Ошибка при отправке данных");
            exit(1);
        }
    }

    if (close(sockfd) < 0) {
        perror("Ошибка при закрытии сокета");
        exit(1);
    }

    return 0;
}
