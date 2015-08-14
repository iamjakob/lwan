#include <stdatomic.h>
#include <stdlib.h>
#include "bounded_queue.h"
//based on
//http://www.1024cores.net/home/lock-free-algorithms/queues/unbounded-spsc-queue

void init_queue(bounded_queue *toinit, uint64_t maxelems) {
    toinit->blocks_t = calloc(maxelems, sizeof(*toinit->blocks_t));
    toinit->blocks_h = toinit->blocks_t;
    toinit->max_size_h = toinit->max_size_t = maxelems;
    atomic_init(&toinit->head, 0);
    atomic_init(&toinit->tail, 0);
    toinit->head_c = toinit->tail_c = 0;
}

//theoreticaly, I guess 0xdeadbeef could be valid values for these...
//but would never happen
void clear_queue(bounded_queue *toclear) {
    //object is invalid, store clearly bad values
    //to help debug
    free(toclear->blocks_t);
    atomic_store(&toclear->head, 0xdeadbeef);
    atomic_store(&toclear->tail, 0xdeadbeef);
    toclear->head_c = toclear->tail_c = 0xdeadbeef;
    toclear->blocks_t = toclear->blocks_h = (void **)0xdeadbeef;
}

char push_many(bounded_queue *into, void **topush, size_t npush) {
    uint64_t ctail = atomic_load_explicit(&into->tail,
                                          memory_order_relaxed);
    size_t modsize = into->max_size_t - 1;
    if (into->head_c + modsize - ctail < npush) {
        into->head_c = atomic_load_explicit(&into->head,
                                            memory_order_acquire);
        if (into->head_c + modsize - ctail < npush)
            return 0;
    }
    void **curdata = into->blocks_t;
    for (size_t i = 0; i < npush; i++) {
        curdata[(ctail + i) & modsize] = topush[i];
    }
    atomic_store_explicit(&into->tail,
                          ctail + npush,
                          memory_order_release);
    return 1;
}

char push(bounded_queue *into, void *topush) {
    uint64_t ctail = atomic_load_explicit(&into->tail,
                                          memory_order_relaxed);
    size_t modsize = into->max_size_t - 1;
    if (into->head_c + modsize == ctail) {
        into->head_c = atomic_load_explicit(&into->head,
                                            memory_order_acquire);
        if (into->head_c + modsize == ctail)
            return 0;
    }
    into->blocks_t[ctail & modsize] = topush;
    atomic_store_explicit(&into->tail,
                          ctail + 1,
                          memory_order_release);
    return 1;
}

char pop_many(bounded_queue *from, void **elems, size_t topop) {
    uint64_t chead = atomic_load_explicit(&from->head,
                                          memory_order_relaxed);
    size_t modsize = from->max_size_t - 1;
    if (from->tail_c - chead < topop) {
        from->tail_c = atomic_load_explicit(&from->tail,
                                            memory_order_acquire);
        if (from->tail_c - chead < topop)
            return 0;
    }
    void **curdata = from->blocks_h;
    for (size_t i = 0; i < topop; i++) {
        elems[i] = curdata[(chead + i) & modsize];
    }
    atomic_store_explicit(&from->head,
                          chead + topop,
                          memory_order_release);
    return 1;
}

char pop(bounded_queue *from, void **elem) {
    uint64_t chead = atomic_load_explicit(&from->head,
                                          memory_order_relaxed);
    size_t modsize = from->max_size_t - 1;
    if (from->tail_c == chead) {
        from->tail_c = atomic_load_explicit(&from->tail,
                                            memory_order_acquire);
        if (from->tail_c == chead)
            return 0;
    }
    *elem = from->blocks_h[chead & modsize];
    atomic_store_explicit(&from->head,
                          chead + 1,
                          memory_order_release);
    return 1;
}
