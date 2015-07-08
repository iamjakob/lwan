#include "page_cache.h"
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGES_IN_BLOCK 1
#define MAX_PAGES 100

#define lwan_likely(x) (!!(x))
#define lwan_unlikely(x) (!(x))
#define lwan_noinline __attribute__ ((noinline))

union mem_page;


typedef union mem_page {
    char memblock[PAGE_SIZE];
    union mem_pages *next;
} mem_page;

static mem_page *get_block() {
    return aligned_alloc(PAGE_SIZE, sizeof(mem_page));
}

static void free_page(mem_page *page) {
    free(page);
}

void clear_page_cache(page_cache *toclear) {
    toclear->num_blocks = 0;
    while (toclear->blocks) {
        void *todel = toclear->blocks;
        toclear->blocks = toclear->blocks->next;
        free_page(todel);
    }
}

void init_page_cache(page_cache *toinit) {
    toinit->num_blocks = 0;
    toinit->blocks = 0;
}

void *retrieve_block(page_cache *from) {
    if (from->blocks) {
        from->num_blocks--;
        void *rblk = from->blocks;
        from->blocks = from->blocks->next;
        return rblk;
    }
    return get_block();
}

void return_block (page_cache *from, void *blk) {
    if (from->num_blocks >= MAX_PAGES)
        free_page(blk);
    else {
        mem_page *inblk = blk;
        inblk->next = from->blocks;
        from->blocks = inblk;
        from->num_blocks++;
    }
}
