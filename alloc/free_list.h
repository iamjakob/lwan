#pragma once
#include "block_cache.h"
#include <stdint.h>
typedef struct { 
    struct slab *partial, *full, *empty;
    block_cache *cache;
    block_sizes which;
    uint16_t num_per_slab;
    uint16_t data_size;
    uint8_t num_empty;

} tracked_free_list;

