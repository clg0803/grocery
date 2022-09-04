source = server.c cook.c
target = server

config = -l pthread -Wall

server: $(source)
	gcc $(source) -o $(target) $(config)


debug: $(source)
	gcc $(source) -o $(target) $(config) -g

.PHONY:clean kill
clean:
	rm -f $(target)

kill:
	pgrep server -x | xargs kill -9