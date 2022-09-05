#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#include "cook.h"

typedef struct ingredient {
    int client_socket;
} ingredient;

void* fire(void* in) {
    char buf[1024];
    ingredient *ing = (ingredient *)in;
    int client_socket = ing->client_socket;
    
    read(client_socket, buf, 1024);
    printf("%s", buf);

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server: DWBServer\r\nContent-Type: text/html;charset=utf-8\r\n\r\n";
    char body[] = "<h1>Hello World<h1>";

    write(client_socket, status, sizeof(status));
    write(client_socket, header, sizeof(header));
    write(client_socket, body, sizeof(body));

    close(client_socket);
}

void cook(int client_socket) {
    pthread_t tid = -1;
    ingredient in = {client_socket};
    int err = pthread_create(&tid, NULL, &fire, (void *) &in);
    if (err) {
        puts("Failed to create a thread to handle requests ...");
    }
}