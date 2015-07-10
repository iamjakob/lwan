#include "page_cache.h"
#include <stdlib.h>

#define PAGE_SIZE 4096
#define PAGES_IN_BLOCK 1
#define MAX_PAGES 100

#define lwan_likely(x) (!!(x))
#define lwan_unlikely(x) (!(x))
#define lwan_noinline __attribute__ ((noinline))

typedef union mem_page {
    char memblock[PAGE_SIZE];
    union mem_page *next;
} mem_page;

struct page_cache{
    union mem_page *blocks;
    size_t num_blocks;
};

static mem_page *get_block() {
    return aligned_alloc(PAGE_SIZE, sizeof(mem_page));
}

static void free_page(mem_page *page) {
    free(page);
}

void clear_page_cache(struct page_cache *toclear) {
    toclear->num_blocks = 0;
    while (toclear->blocks) {
        void *todel = toclear->blocks;
        toclear->blocks = toclear->blocks->next;
        free_page(todel);
    }
}

static void init_page_cache(struct page_cache *toinit) {
    toinit->num_blocks = 0;
    toinit->blocks = NULL;
}

void *retrieve_block(struct page_cache *from) {
    if (from->blocks) {
        from->num_blocks--;
        void *rblk = from->blocks;
        from->blocks = from->blocks->next;
        return rblk;
    }
    return get_block();
}

void return_block (struct page_cache *from, void *blk) {
    if (from->num_blocks >= MAX_PAGES)
        free_page(blk);
    else {
        mem_page *inblk = blk;
        inblk->next = from->blocks;
        from->blocks = inblk;
        from->num_blocks++;
    }
}

struct page_cache *get_cache_for_thread() {
    static _Thread_local struct page_cache threadcache;
    static _Thread_local char hasinit = 0;
    if (!hasinit)
        init_page_cache(&threadcache);
    return &threadcache;
};
