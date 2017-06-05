CC=gcc
OPTIONS=-Wall -gdwarf-3
HEADERS=-I./include -I./vendor

.PHONY: all
all: tests

epoll_playground: test/epoll_playground.c
	$(CC) $(OPTIONS) $(HEADERS) -o epoll_playground test/epoll_playground.c
	mkfifo test.fifo

process_runner: test/process_runner.c
	$(CC) $(OPTIONS) -o process_runner test/process_runner.c

host_filter:
	$(CC) $(OPTIONS) $(HEADERS) -o host_filter -lm src/host_filter.c src/file_mapper.c vendor/cJSON.c

vmbuf_test: src/vmbuf.c test/vmbuf_test.c
	$(CC) $(OPTIONS) $(HEADERS) -o vmbuf_test test/vmbuf_test.c src/vmbuf.c

.PHONY: tests
tests: epoll_playground process_runner host_filter vmbuf_test

.PHONY: clean
clean:
	rm -f epoll_playground
	rm -f test.fifo
	rm -f process_runner
	rm -f host_filter
	rm -f vmbuf_test
