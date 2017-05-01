CC = gcc
OPTIONS = -Wall -gdwarf-3

.PHONY: all
all: epoll_playground process_runner

epoll_playground:
	$(CC) $(OPTIONS) -o epoll_playground src/epoll_playground.c
	mkfifo test.fifo

process_runner:
	$(CC) $(OPTIONS) -o process_runner src/process_runner.c

.PHONY: clean
clean:
	rm -f epoll_playground
	rm -f test.fifo
	rm -f process_runner
