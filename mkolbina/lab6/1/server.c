#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int server_sock, client_sock, port_num, client_len, recv_len;
    char *buffer = (char *) malloc(BUF_SIZE * sizeof(char));
    struct sockaddr_in server_addr, client_addr;

    // Создаем UDP-сокет
    server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_sock < 0) {
        perror("Ошибка создания сокета");
        free(buffer);
        close(server_sock);

        exit(1);
    }

    // Задаем параметры сервера
    memset(&server_addr, 0, sizeof(server_addr));
    port_num = 8888;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port_num);

    // Привязываем сокет к адресу и порту
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки сокета");
        free(buffer);
        close(server_sock);
        exit(1);
    }

    printf("Сервер запущен и ожидает сообщения...\n");

    while (1) {
        // Принимаем сообщение от клиента
        client_len = sizeof(client_addr);
        recv_len = recvfrom(server_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Ошибка приема сообщения");
            free(buffer);
            close(server_sock);
            exit(1);
        }

        // Выводим информацию о клиенте и принятом сообщении
        printf("Получено сообщение от %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Сообщение: %s\n", buffer);

        // Отправляем сообщение обратно клиенту
        if (sendto(server_sock, buffer, recv_len, 0, (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("Ошибка отправки сообщения");
            free(buffer);
            close(server_sock);
            exit(1);
        }
    }

    // Закрываем сокет
    close(server_sock);
    free(buffer);

    return 0;
}
