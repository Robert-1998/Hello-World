#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

    // 1. Создаём сокет
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Привязываем сокет к порту
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Сервер запущен на порту %d\n", PORT);

    // 3. Ждём подключения клиента
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    printf("Клиент подключён\n");

    // 4. Получаем данные от клиента
    while (1) {
        read(new_socket, buffer, 1024);
        printf("Получено: %s\n", buffer);
        send(new_socket, "Сервер получил данные", strlen("Сервер получил данные"), 0);
    }

    return 0;
}