#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

struct hashtable {
    void *hash;
    void *data;
    size_t size;
};

uint32_t murmur3_32();

int hashtable_init(struct hashtable *ht);


#endif
