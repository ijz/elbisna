#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MB (1 * 1024 * 1024)
static int open_file_nb(const char *filename) {
    int fd = open(filename, O_RDONLY | O_NONBLOCK);
    return fd;
}

int main(int argc, char **argv) {
    int epoll_fd, nfds;
    struct epoll_event ev_rcv;
    if (0 > (epoll_fd = epoll_create1(0))) {
        perror("epoll_create1");
        exit(-1);
    }
    printf("file: %s\n", argv[1]);
    int ffd = open_file_nb(argv[1]);
    struct epoll_event ev = {
        .events = EPOLLIN | EPOLLET,
        .data.fd = ffd
    };
    if (0 > epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ffd, &ev)) {
        perror("epoll_ctl");
        exit(-1);
    }
    size_t buf_size = 1 * MB;
    void *file = malloc(buf_size);
EPOLL_LOOP: for (;;) {
        if (0 > (nfds = epoll_wait(epoll_fd, &ev_rcv, 1, -1))) {
            perror("epoll_wait");
            exit(-1);
        }
        if (0 == nfds) continue;
        if (ffd != ev_rcv.data.fd) {
            printf("weird. ffd: %d, ev_rcv.data.fd: %d\n", ffd, ev_rcv.data.fd);
            continue;
        }
        ssize_t rs;
        ssize_t total = 0;
        while (0 < (rs = read(ffd, file + total, buf_size))) {
            total += rs;
            if (total >= buf_size) {
                void *temp = malloc(buf_size * 2);
                memcpy(temp, file, buf_size);
                // arrrrrrrr
                free(file);
                file = temp;
                buf_size *= 2;
            }
            if (rs < buf_size) {
                // partial read
                printf("event! read: %.*s\n", (int)total, (char *)file);
                goto EPOLL_LOOP;
            }

        }
        if (0 >= rs) {
            perror("read");
            if (EAGAIN == errno || 0 == errno) continue;
            exit(-1);
        }

    }
}
