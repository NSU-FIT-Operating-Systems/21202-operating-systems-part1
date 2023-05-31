#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define EXIT_WORD "Выход"
#define BUF_SIZE 1024

void FreeBuf(char* buffer, int server_sock) {
    free(buffer);
    if (close(server_sock) == -1) {
        perror("Ошибка закрытия сокета");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    int server_sock, port_num, recv_len;
    unsigned int client_len;
    char *buffer = (char *) malloc(BUF_SIZE * sizeof(char));
    if (buffer == NULL) {
        perror("Ошибка выделения памяти");
        exit(1);
    }
    struct sockaddr_in server_addr, client_addr;

    // Создаем UDP-сокет
    server_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_sock < 0) {
        perror("Ошибка создания сокета");
        FreeBuf(buffer, server_sock);
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
        FreeBuf(buffer, server_sock);
        exit(1);
    }

    printf("Сервер запущен и ожидает сообщения...\n");

    while (1) {
        // Принимаем сообщение от клиента
        client_len = sizeof(client_addr);
        recv_len = recvfrom(server_sock, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (recv_len < 0) {
            perror("Ошибка приема сообщения");
            FreeBuf(buffer, server_sock);
            exit(1);
        }
        if (strncmp(buffer, EXIT_WORD, 5) == 0) {
            printf("Завершение работы сервера\n");
            FreeBuf(buffer, server_sock);
            break;
        }

        // Выводим информацию о клиенте и принятом сообщении
        printf("Получено сообщение от %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Сообщение: %s\n", buffer);

        // Отправляем сообщение обратно клиенту
        if (sendto(server_sock, buffer, recv_len, 0, (struct sockaddr *)&client_addr, client_len) < 0) {
            perror("Ошибка отправки сообщения");
            FreeBuf(buffer, server_sock);
            exit(1);
        }
    }


    return 0;
}
