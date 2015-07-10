#pragma once
#include "block_cache.h"
#include <stdint.h>

typedef struct { 
    struct slab *partial, *full, *empty;
    block_cache *cache;
    uint16_t which;
    uint16_t num_per_slab;
    uint16_t data_size;
    uint16_t num_empty;

} tracked_free_list;


void tracked_free(tracked_free_list *into, void *freeptr);
