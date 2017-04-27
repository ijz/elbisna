.PHONY: all
all: epoll_playground

epoll_playground:
	gcc -Wall -gdwarf-3 -o epoll_playground src/epoll_playground.c
	mkfifo test.fifo

.PHONY: clean
clean:
	rm -f epoll_playground
	rm -f test.fifo
