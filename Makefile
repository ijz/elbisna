CC=gcc
OPTIONS=-Wall -gdwarf-3
HEADERS=-I./include -I./vendor

.PHONY: all
all: epoll_playground process_runner

epoll_playground:
	$(CC) $(OPTIONS) -o epoll_playground src/epoll_playground.c
	mkfifo test.fifo

process_runner:
	$(CC) $(OPTIONS) -o process_runner src/process_runner.c

host_filter:
	$(CC) $(OPTIONS) $(HEADERS) -o host_filter -lm src/host_filter.c src/file_mapper.c vendor/cJSON.c

.PHONY: clean
clean:
	rm -f epoll_playground
	rm -f test.fifo
	rm -f process_runner
	rm -f host_filter
