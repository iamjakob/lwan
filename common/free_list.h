#pragma once
#include "block_cache.h"
#include <stdint.h>

typedef struct { 
    union chunk *first_open;
    struct block *blocks;
    block_cache *cache;
    uint32_t nblocks;
    uint16_t which;
    uint16_t data_size;
} free_list;


void free_list_free(free_list *into, void *freeptr);
