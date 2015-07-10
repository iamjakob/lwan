#pragma once
#include <stddef.h>
#include <stdint.h>

//pages manager acts as a buffer between malloc and the
//pools/free-lists, allow better memory resuse and far less actual

//eventually, probably should have some global manager so that threads
//can share with each toher to an extent. But premature optimization
//is bad guys.


struct page_cache;

void clear_page_cache(struct page_cache *toclear);

void *retrieve_block(struct page_cache *from);

void return_block(struct page_cache *to, void *blk);

struct page_cache *get_cache_for_thread();
