#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "cook.h"

void* fire(void* client_socket) {
    char buf[1024];
    read(client_socket, buf, 1024);
    printf("%s", buf);

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
    char body[] = "<html><head><title>C语言构建小型Web服务器</title></head><body><h2>欢迎</h2><p>Hello，World</p></body></html>";

    write(client_socket, status, sizeof(status));
    write(client_socket, header, sizeof(header));
    write(client_socket, body, sizeof(body));

    close(client_socket);
}

void cook(int client_socket) {
    pthread_t tid;
    int err = pthread_create(&tid, NULL, fire, &client_socket);
    if (err) {
        // 
        puts("Failed to create a thread to handle requests ...");
        return;
    }
}