#pragma once

#include <stddef.h>
//each result actually gets size - 32, bytes
//bookkeeping is kept in power of two sizes to make
//alignment easier
typedef enum {
    SMALL = 512,
    MEDIUM = 1024,
    LARGE = 2048,
    PAGE = 4096
} block_sizes;

typedef struct block {
    struct block *next; //also usable when outside of the alloc...
    struct block *act_next_; //used when clearing threadlocal
    struct block *act_prev_; //used when clearing threadlocal
    void *data;
} block;

typedef struct {
    block *blocks;
    block *used;
    size_t num_blocks;
    block_sizes which;
} block_cache;

block *get_block(block_cache *cache);

block *return_block(block_cache *cache, block *blk);

void clear_block_cache(block_cache *inblock);

void clear_all_block_cache(block_cache *inblock);

block_cache *get_thread_local_cache(block_sizes which);

void clear_threadlocals();

void clear_all_threadlocals();
