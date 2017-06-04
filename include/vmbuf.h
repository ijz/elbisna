#ifndef _VMBUF_H_
#define _VMBUF_H_

#define _GNU_SOURCE

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

#define VMBUF_EMPTY { NULL, 0, 0 }
#define VMBUF_PAGE_SIZE getpagesize()
#define VMBUF_PAGE_ALIGN_MASK (~(VMBUF_PAGE_SIZE - 1))
#define VMBUF_PAGE_ALIGN(x) (((x) + VMBUF_PAGE_SIZE - 1) & VMBUF_PAGE_ALIGN_MASK)
#define VMBUF_AVAIL(x) (((x)->capacity - (x)->write_loc) + 1)

struct vmbuf {
    char *mem;
    size_t write_loc;
    size_t capacity;
};

int vmbuf_init(struct vmbuf *buf);
int vmbuf_init2(struct vmbuf *buf, size_t size);
int vmbuf_free(struct vmbuf *buf);
int vmbuf_resize(struct vmbuf *buf, size_t new_size);
int vmbuf_double_if_full(struct vmbuf *buf);

char *vmbuf_mem(struct vmbuf *buf);

char *vmbuf_wloc(struct vmbuf *buf);
size_t vmbuf_wlocpos(struct vmbuf *buf);
size_t vmbuf_wavail(struct vmbuf *buf);
int vmbuf_winc(struct vmbuf *buf, size_t by);

int vmbuf_memcpy(struct vmbuf *buf, void *mem, size_t size);
int vmbuf_sprintf(struct vmbuf *buf, const char *format, ...);
int vmbuf_vsprintf(struct vmbuf *buf, const char *format, va_list ap);

#endif
