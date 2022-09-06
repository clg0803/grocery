server: server.c cook.c
	gcc server.c cook.c -o server -lpthread 

server-epoll: server_epoll.c cook.c
	gcc server_epoll.c cook.c -o server_epoll 

.PHONY:clean kill
clean:
	rm -f server_epoll server

kill:
	pgrep server -x | xargs kill -9
	pgrep server_epoll -x | xargs kill -9