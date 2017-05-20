#ifndef _FILE_MAPPER_H_
#define _FILE_MAPPER_H_

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define FM_EMPTY { NULL, 0, 0, -1 }

struct file_mapper {
    char *mem;
    size_t size;
    size_t capacity;
    int fd;
};

int fm_init_ro(struct file_mapper *fm, const char *path);
int fm_init(struct file_mapper *fm, const char *path, int oflags, int mmap_prot, int mmap_flags);
int fm_init_fd(struct file_mapper *fm, int fd, int mmap_prot, int mmap_flags);
int fm_free(struct file_mapper *fm);

#endif
