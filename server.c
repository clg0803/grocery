#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "cook.h"

#define PORT 8080

int main() {
	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family 		= AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port		= htons(PORT);

	bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));

	listen(server_socket, 5);

	for (;;) {
		// blocked
		int client_socket = accept(server_socket, NULL, NULL);
		cook(client_socket);
	}
	// nerver reach there
    close(server_socket);

    return 0;
}
