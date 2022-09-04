source = server.c cook.c
target = server

server: $(source)
	clang $(source) -o $(target)

.PHONY:clean
clean:
	rm -f $(target)