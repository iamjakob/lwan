#include "free_list.h"
#include <assert.h>

typedef union chunk {
    union chunk *next;
    char data[1]; //char c[] not allowed in union
} chunk;

static void *add_block(free_list *from) {
    block *blk = get_block(from->cache);
    uint16_t elems_per_block = data_space(from->which) / from->data_size;
    chunk *curchunk = blk->data;
    curchunk++;
    for (size_t i = 1; i < elems_per_block; i++) {
        chunk *next = (chunk *)((char *)curchunk + from->data_size);
        curchunk->next = next;
        curchunk = next;
    }
    curchunk->next = NULL;
    blk->next = from->blocks;
    from->blocks = blk;
    from->nblocks++;
    return blk->data;
}

void *free_list_alloc(free_list *from) {
    chunk *first = from->first_open;
    if (first)
        return add_block(from);
    void *rdata = first->data;
    from->first_open = first->next;
    return rdata;
}

void free_list_free(free_list *into, void* freeptr) {
    if (freeptr) {
        ((chunk *)freeptr)->next = into->first_open;
        into->first_open = freeptr;
    }
}

void clear_free_list(free_list *toclear) {
    while (toclear->blocks) {
        block *todel = toclear->blocks;
        toclear->blocks = toclear->blocks->next;
        return_block(toclear->cache, todel);
    }
    toclear->nblocks = 0;
    toclear->first_open = NULL;
}

static uint16_t get_data_size(uint16_t in) {
    if (in < sizeof(chunk))
        return sizeof (chunk);
    if (!(in % sizeof(chunk)))
        return in;
    return in + sizeof(chunk) - (in % sizeof(chunk));
}

void init_free_list(free_list *toinit, uint16_t alloc_size, block_sizes which) {
    toinit->which = which;
    toinit->blocks = NULL;
    toinit->first_open = NULL;
    toinit->nblocks = 0;
    toinit->data_size = get_data_size(alloc_size);
    assert(2*toinit->data_size < data_space(which));
}
