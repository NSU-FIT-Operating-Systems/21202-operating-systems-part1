#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
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
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("Ошибка при преобразовании IP-адреса");
        exit(1);
    }

    printf("Введите сообщение для отправки серверу:\n");
    if (fgets(buffer, MAX_BUFFER_SIZE, stdin) == NULL) {
        perror("Ошибка при чтении ввода");
        exit(1);
    }

    // Отправка данных серверу
    int numBytesSent = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (numBytesSent < 0) {
        perror("Ошибка при отправке данных");
        exit(1);
    }

    // Получение данных от сервера
    int numBytesReceived = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
    if (numBytesReceived < 0) {
        perror("Ошибка при приеме данных");
        exit(1);
    }

    printf("Полученное сообщение от сервера: %s\n", buffer);

    if (close(sockfd) < 0) {
        perror("Ошибка при закрытии сокета");
        exit(1);
    }

    return 0;
}
