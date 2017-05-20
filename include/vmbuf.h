#ifndef _VMBUF_H_
#define _VMBUF_H_

#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define VMBUF_EMPTY { NULL, 0, 0, 0 }
#define VMBUF_PAGE_SIZE getpagesize()
#define VMBUF_PAGE_ALIGN_MASK (~(VMBUF_PAGE_SIZE - 1))
#define VMBUF_PAGE_ALIGN(x) (((x) + VMBUF_PAGE_SIZE - 1) & VMBUF_PAGE_ALIGN_MASK)

struct vmbuf {
    void *mem;
    size_t read_loc;
    size_t write_loc;
    size_t capacity;
};

int vmbuf_init(struct vmbuf *buf);
int vmbuf_init2(struct vmbuf *buf, size_t size);
int vmbuf_free(struct vmbuf *buf);
int vmbuf_resize(struct vmbuf *buf, size_t new_size);

size_t vmbuf_wloc(struct vmbuf *buf);
size_t vmbuf_rloc(struct vmbuf *buf);
int vmbuf_memcpy(struct vmbuf *buf, void *mem, size_t size);
int vmbuf_sprintf(struct vmbuf *buf, const char *format, ...);
int vmbuf_snprintf(struct vmbuf *buf, size_t size, const char *format, ...);

#endif
