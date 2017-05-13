#include "host_filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int parse_host(cJSON *host) {
    if (!host || !host->child) {
        printf("bad host\n");
        return -1;
    }
    cJSON *child = host->child;
    while (child) {
        printf("%s:%s\n", child->string, child->valuestring);
        child = child->next;
    }
    return 0;
}

int parse_hostfile(struct file_mapper *hostfile) {
    cJSON *root = cJSON_Parse(hostfile->mem);
    int num_hosts = cJSON_GetArraySize(root);
    size_t i;
    for (i = 0; i < num_hosts; ++i) {
        cJSON *item = cJSON_GetArrayItem(root, i);
        parse_host(item);
        printf("\n\n");
    }
    return 0;
}

int main(int argc, char **argv) {
    const char *file = argv[1];
    struct file_mapper fm = FM_EMPTY;
    if (0 > fm_init_ro(&fm, file)) {
        perror("fm_init_ro");
        return -1;
    }
    parse_hostfile(&fm);
    if (0 > fm_free(&fm)) {
        perror("fm_free");
        return -1;
    }
    return 0;
}
