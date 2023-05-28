#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int client_sock, port_num, server_len, send_len, recv_len;
    char buffer[BUF_SIZE];
    struct sockaddr_in server_addr;

    // Создаем UDP-сокет
    client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_sock < 0) {
        perror("Ошибка создания сокета");
        exit(1);
    }

    // Задаем параметры сервера
    memset(&server_addr, 0, sizeof(server_addr));
    port_num = 8888;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_num);
    if (inet_aton("127.0.0.1", &server_addr.sin_addr) == 0) {
        perror("Ошибка адреса сервера");
        exit(1);
    }

    printf("Клиент запущен\n");

    while (1) {
        // Вводим сообщение для отправки
        printf("Введите сообщение: ");
        fgets(buffer, BUF_SIZE, stdin);

        // Отправляем сообщение серверу
        server_len = sizeof(server_addr);
        send_len = sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, server_len);
        if (send_len < 0) {
            perror("Ошибка отправки сообщения");
            exit(1);
        }

        // Принимаем ответ от сервера
        recv_len = recvfrom(client_sock, buffer, BUF_SIZE, 0, NULL, NULL);
        if (recv_len < 0) {
            perror("Ошибка приема сообщения");
            exit(1);
        }

        // Выводим принятый ответ
        printf("Получен ответ: %s\n", buffer);
    }

    // Закрываем сокет
    close(client_sock);

    return 0;
}