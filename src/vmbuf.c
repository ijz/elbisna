#include "vmbuf.h"

int vmbuf_init(struct vmbuf *buf) {
    return vmbuf_init2(buf, VMBUF_PAGE_SIZE);
}

int vmbuf_init2(struct vmbuf *buf, size_t size) {
    size_t init_size = VMBUF_PAGE_ALIGN(size);
    void *m = mmap(NULL, init_size, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_SHARED, 0, 0);
    if (MAP_FAILED == m) {
        perror("mmap");
        return -1;
    }
    *buf = VMBUF_EMPTY;
    buf->mem = m;
    buf->capacity = init_size;
    return 0;
}

int vmbuf_free(struct vmbuf *buf) {
    int res;
    if (0 > (res = munmap(buf->mem, buf->capacity)))
        perror("munmap");
    *buf = VMBUF_EMPTY;
    return res;
}

int vmbuf_resize(struct vmbuf *buf, size_t new_size) {
    voic *m = mremap(buf->mem, buf->capacity, new_size, MREMAP_MAYMOVE);
    if (MAP_FAILED == m) {
        perror("mremap");
        return -1;
    }
    return 0;
}

inline size_t vmbuf_wloc(struct vmbuf *buf) {
    return buf->write_loc;
}

inline size_t vmbuf_rloc(struct vmbuf *buf) {
    return buf->read_loc;
}

int vmbuf_memcpy(struct vmbuf *buf, void *mem, size_t size) {
    size_t wloc = buf->write_loc;
    if (buf->capacity < wloc + size) {
        // double size
        if (0 > vmbuf_resize(buf, buf->capacity * 2)) {
            perror("vmbuf_resize");
            return -1;
        }
    }
    memcpy(buf->mem + wloc, mem, size);
    buf->write_loc += size;
    return 0;
}

int vmbuf_sprintf(struct vmbuf *buf, const char *format, ...) {

    return 0;
}

int vmbuf_nsprintf(struct vmbuf *buf, size_t size, const char *format, ...) {
    return 0;
}
