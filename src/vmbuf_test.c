#include "vmbuf.h"

#include <stdio.h>

int main(int argc, char **argv) {
    struct vmbuf buf = (struct vmbuf)VMBUF_EMPTY;
    vmbuf_init(&buf);
    int i;
    for (i = 0; i < 10000; ++i) {
        size_t wloc = vmbuf_wlocpos(&buf);
        vmbuf_sprintf(&buf, "this is test %d", i);
        printf("capacity:%zu wloc:%zu string:%s\n",
               buf.capacity, buf.write_loc,
               vmbuf_mem(&buf) + wloc);

    }
    vmbuf_free(&buf);
}
