#include "vmbuf.h"

int vmbuf_init(struct vmbuf *buf) {
    return vmbuf_init2(buf, VMBUF_PAGE_SIZE);
}

int vmbuf_init2(struct vmbuf *buf, size_t size) {
    size_t init_size = VMBUF_PAGE_ALIGN(size);
    /* MAP_PRIVATE required for ANONYMOUS mmaps due mremap
       does not grow the shared memory file kernel creates
       when MAP_SHARED is in use.
     */
    char *m = mmap(NULL, init_size, PROT_READ | PROT_WRITE,
                   MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (MAP_FAILED == m) {
        perror("mmap");
        return -1;
    }
    *buf = (struct vmbuf)VMBUF_EMPTY;
    buf->mem = m;
    buf->capacity = init_size;
    return 0;
}

int vmbuf_free(struct vmbuf *buf) {
    int res;
    if (0 > (res = munmap(buf->mem, buf->capacity)))
        perror("munmap");
    *buf = (struct vmbuf)VMBUF_EMPTY;
    return res;
}

int vmbuf_resize(struct vmbuf *buf, size_t new_size) {
    char *m = mremap(buf->mem, buf->capacity, new_size, MREMAP_MAYMOVE);
    if (MAP_FAILED == m) {
        perror("mremap");
        return -1;
    }
    buf->mem = m;
    buf->capacity = new_size;
    return 0;
}

int vmbuf_double_if_full(struct vmbuf *buf) {
    if (buf->write_loc < buf->capacity)
        return 0;
    return vmbuf_resize(buf, buf->capacity * 2);
}

inline char *vmbuf_mem(struct vmbuf *buf) {
    return buf->mem;
}

inline char *vmbuf_wloc(struct vmbuf *buf) {
    return buf->mem + buf->write_loc;
}

inline size_t vmbuf_wlocpos(struct vmbuf *buf) {
    return buf->write_loc;
}

inline size_t vmbuf_wavail(struct vmbuf *buf) {
    return buf->capacity - buf->write_loc;
}

inline int vmbuf_winc(struct vmbuf *buf, size_t by) {
    buf->write_loc += by;
    return vmbuf_double_if_full(buf);
}

int vmbuf_memcpy(struct vmbuf *buf, void *mem, size_t size) {
    size_t loc = vmbuf_wlocpos(buf) + size;
    if (buf->capacity < loc) {
        // double size
        if (0 > vmbuf_resize(buf, buf->capacity * 2)) {
            perror("vmbuf_resize");
            return -1;
        }
    }
    memcpy(vmbuf_wloc(buf), mem, size);
    buf->write_loc += size;
    return 0;
}

int vmbuf_sprintf(struct vmbuf *buf, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    int r = vmbuf_vsprintf(buf, format, ap);
    va_end(ap);
    return r;
}

int vmbuf_vsprintf(struct vmbuf *buf, const char *format, va_list ap) {
    int written;
    while (1) {
        size_t avail = vmbuf_wavail(buf);
        va_list ap2;
        va_copy(ap2, ap);
        written = vsnprintf(vmbuf_wloc(buf), avail, format, ap2);
        va_end(ap2);
        if (0 > written) {
            perror("vsnprintf");
            return -1;
        }
        if (written < avail)
            break;
        // doesn't fit, double capacity
        if (0 > vmbuf_resize(buf, buf->capacity * 2))
            return -1;
    }
    buf->write_loc += written;

    return written;
}
