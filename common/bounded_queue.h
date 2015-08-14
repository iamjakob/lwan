#pragma once
#include <stdint.h>
#include <stddef.h>

//technically, this implementation has an integer overflow
//but would require 580 years of nonstop pushing at 1e9 pushes/sec
//so never happening.

//still working on groking the poll structure to see if
//this could provide better throughput in a not-too-complex manner
typedef struct {
    _Atomic uint64_t head;
    uint64_t tail_c;
    //store copies of state in cache line of head/tail data
    //cheaper than having a third cache line for shared stuff
    size_t max_size_h;
    void **blocks_h;

    char cache_buff[64];

    _Atomic uint64_t tail;
    uint64_t head_c;
    size_t max_size_t;
    void **blocks_t;
    
} bounded_queue;

char push(bounded_queue *into, void *data);
char pop(bounded_queue *into, void **elem);
void init_queue(bounded_queue *toinit, uint64_t maxelems);
void clear_queue(bounded_queue *toclear);
