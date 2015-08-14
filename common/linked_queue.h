#pragma once
#include <stddef.h>

//an array implementation requires more synchronization
//and every push/pop has to share a cache line

//still working on groking the poll structure to see if
//this could provide better throughput
typedef struct {

    //used by consumer to pull data
    struct node * _Atomic contail;
    
    //enfore difference cache line
    struct node *node_block;
    char buffer[64 - sizeof(struct node *) - sizeof(node * _Atomic)];

    //used by producer
    struct node * _Atomic head, *first, *tail_cpy;

} bounded_queue;
