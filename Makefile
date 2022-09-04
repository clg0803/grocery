server: server.c
	gcc -Wall server.c -o server

.PHNOY:
clean:
	rm -f server
