#include "file_mapper.h"
#include "vmbuf.h"

inline int fm_init_ro(struct file_mapper *fm, const char *path) {
    return fm_init(fm, path, O_RDONLY | O_CLOEXEC, PROT_READ, MAP_SHARED);
}

int fm_init(struct file_mapper *fm, const char *path,
            int oflags, int mmap_prot, int mmap_flags) {
    int fd;
    if (0 > (fd = open(path, oflags))) {
        perror("open");
        return -1;
    }
    return fm_init_fd(fm, fd, mmap_prot, mmap_flags);
}

int fm_init_fd(struct file_mapper *fm, int fd, int mmap_prot, int mmap_flags) {
    struct stat st;
    if (0 > fstat(fd, &st)) {
        perror("fstat");
        return -1;
    }
    fm->fd = fd;
    fm->size = st.st_size;
    fm->capacity = VMBUF_PAGE_ALIGN(st.st_size);
    if (MAP_FAILED == (fm->mem = (char *)mmap(NULL, fm->capacity, mmap_prot, mmap_flags, fd, 0))) {
        perror("mmap");
        return -1;
    }
    return 0;
}

int fm_free(struct file_mapper *fm) {
    if (!fm || !fm->mem)
        return 0;
    int res;
    if (0 > (res = munmap(fm->mem, fm->capacity)))
        perror("munmap");
    *fm = (struct file_mapper)FM_EMPTY;
    return res;
}
