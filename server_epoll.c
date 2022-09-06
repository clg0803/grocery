#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include "cook.h"

#define MAX_EVENTS 10
#define EXIT_FAILURE 1
#define PORT 8080


/**
* examples from: Manual page epoll(7)
**/

int main() {
    struct epoll_event ev, events[MAX_EVENTS];
    int listen_sock, conn_sock, nfds, epollfd;

    // create() bind() listen()
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family 		= AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port		= htons(PORT);

	bind(listen_sock, (struct sockaddr*) &server_addr, sizeof(server_addr));

	listen(listen_sock, 5);

    // init epoll instance
    if ((epollfd = epoll_create1(0)) == -1) {
        printf("epoll_create1");
        exit(EXIT_FAILURE);
    }

    // register listen_sock to epoll_env
    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;

    // register env
    if (epoll_ctl(epollfd, EPOLL_CTL_ADD, listen_sock, &ev) == -1) {
        printf("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    printf("🛒 Grocery(epoll) init success ... \nListen on %d \n", PORT);

    for (;;) {
        // block there
        if ((nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1)) == -1) {
            printf("epoll_wait");
            exit(EXIT_FAILURE);
        }
        /*
        * use ET
        * the call to epoll_wait will probably hang
        * despite the available data still present in the file input buffer
        * 
        * An application  that  employs the EPOLLET flag should use nonblocking file
        * descriptors to avoid having a blocking read or write starve a task that  is
        * handling  multiple  file descriptors.
        */
        for (int n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listen_sock) {
                conn_sock = accept(listen_sock, NULL, NULL);
                if (conn_sock == -1) {
                    printf("accept");
                    exit(EXIT_FAILURE);
                }
                // set non_blocking
                fcntl(conn_sock, 
                    F_SETFL, fcntl(conn_sock,F_GETFL,0) | O_NONBLOCK);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = conn_sock;
                if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
                    printf("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
                }
            } else {
                ingredient in = {events[n].data.fd};
                cook((void *) &in);
            }
        }
    }
    return 0;
}