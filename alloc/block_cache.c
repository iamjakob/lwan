#include "block_cache.h"
#include <stdlib.h>
#include <assert.h>
#define NUM_SIZES 4
#define MAX_HOLD 5

//Block cache manages memory blocks that are a subset of page sizes
//They are less proactive about freeing memory, and act as a fashion for
//other allocators to deal out memory in a non page-chunk size

static _Thread_local block_cache blocks[NUM_SIZES];
static _Thread_local char isinit;

static void init_block_cache(block_cache *cache, block_sizes which) {
    cache->blocks = NULL;
    cache->used = NULL;
    cache->num_blocks = 0;
    cache->which = which;
}

static void free_block(block *inblock) {
    free(inblock->data);
}

static block *create_block (block_sizes which) {
    size_t bsize = which - sizeof(block);
    void *data;
    if (which == PAGE)
        data = aligned_alloc(PAGE, PAGE);
    else
        data = aligned_alloc(64, which);

    if (!data)
        return NULL;
    block *blkpos = (block *)((char *)data + which - sizeof(block));
    blkpos->next = NULL;
    blkpos->data = data;
    return blkpos;
}

block *get_block(block_cache *cache) {
    block *retblk;
    if (cache->blocks) {
        retblk = cache->blocks;
        cache->blocks = cache->blocks->next;
        cache->blocks -= 1;
    }
    else {
        retblk = create_block (cache->which);
    }
    if (!retblk)
        return NULL;
    
    retblk->act_next_ = cache->used;
    cache->used->act_prev_ = retblk;
    retblk->act_prev_ = NULL;
    return retblk;
    
}

block *return_block(block_cache *cache, block *blk) {
    block *an, *ap;
    an = blk->act_next_;
    ap = blk->act_prev_;
    if (an)
        an->act_prev_ = ap;
    if (ap)
        ap->act_next_ = an;
    blk->act_next_ = blk->act_prev_ = NULL;
    blk->next = cache->blocks;
    cache->blocks = blk->next;
    cache->num_blocks += 1;
}
    
void clear_block_cache(block_cache *inblock) {
    inblock->num_blocks = 0;
    while (inblock->blocks) {
        block *tofree = inblock->blocks;
        inblock->blocks = inblock->blocks->next;
        free_block(tofree);
    }
}

void clear_all_block_cache(block_cache *inblock) {
    clear_block_cache (inblock);
    while (inblock->used) {
        block *tofree = inblock->used;
        inblock->used = inblock->used->act_next_;
        free_block(tofree);
    }
}

static size_t get_ind(block_sizes s) {
    switch (s) {
    case SMALL:
        return 0;
    case MEDIUM:
        return 1;
    case LARGE:
        return 2;
    case PAGE:
        return 3;
    default:
        assert(0);
        return 3;
    }
}

static size_t sizelst[] = {
    SMALL,
    MEDIUM,
    LARGE,
    PAGE
};

//since threads are only used at program open/shutdown,
//We don't need to ad any blocks into the mix
block_cache *get_thread_local_cache(block_sizes which) {
    size_t ind = get_ind(which);
    block_cache *getem = blocks;
    if (!isinit) {
        for (size_t i = 0; i < NUM_SIZES; i++) {
            init_block_cache(getem + i, sizelst[i]);
        }
        isinit = 1;
    }
    return blocks + ind;
}

void clear_threadlocals() {
    if (isinit) {
        block_cache *getem = blocks;
        for (size_t i = 0; i < NUM_SIZES; i++) {
            clear_block_cache(getem + i);
        }
    }
}

void clear_all_threadlocals() {
    if (isinit) {
        block_cache *getem = blocks;
        for (size_t i = 0; i < NUM_SIZES; i++) {
            clear_all_block_cache(getem + i);
        }
    }
}
