#pragma once
#include <stddef.h>
#include <stdint.h>

//pages manager acts as a buffer between malloc and the
//pools/free-lists, allow better memory resuse and far less actual

//eventually, probably should have some global manager so that threads
//can share with each toher to an extent. But premature optimization
//is bad guys.

typedef struct {
    union mem_page *blocks;
    size_t num_blocks;
    size_t canary;
} page_cache;

void init_page_cache(page_cache *toinit);

void clear_page_cache(page_cache *toclear);

void *retrieve_block(page_cache *from);

void return_block(page_cache *to, void *blk);
