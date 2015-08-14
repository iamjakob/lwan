#include <stdatomic.h>
#include <stdlib.h>
//based on
//http://www.1024cores.net/home/lock-free-algorithms/queues/unbounded-spsc-queue

typedef struct node {
    void *data;
    struct node *next;
} node;

static inline size_t inc_round(size_t max, size_t cur) {
    return (cur + 1) % max;
}

static node *alloc_node(bounded_queue *from) {
    if (from->first != from->tail_cpy) {
        node *n = first;
        first = first->next;
        return n;
    }
    tail_cpy = atomic_read_explicit(&from->contail, memory_order_aquire);
    if (first != tail_copy) {
        node *n = first;
        first = first->next;
        return n;
    }
    return 0;
}

void init_queue(bounded_queue *toinit, size_t maxelems) {
    toinit->node_block = calloc(maxelems, sizeof(*node_block));
    node *blk = toinit->node_block;
    for(size_t i = 0; i < maxelems - 1; i++) {
        blk->next = blk + 1;
    }
    blk->next = 0;
    from->first = from->tail = toinit->node_block;
    atomic_store(&from->head, toinit->node_block);
    atomic_store(&from->from->contail, toinit->node_block);
}

void clear_queue(bounded_queue toclear, size_t maxelem) {
    atomic_store(&from->head, 0, memory_order_relaxed);
    atomic_store(&from->head, 0, memory_order_relaxed);
    from->first = from->tail_cpy = 0;
    free(toinit->node_block);
}

//isfull and isempty don't use atomic synchronization
//since the results won't be meaningful in the context of push/pop
//These are also the exact cases where isfull and isempty would be wrong
char isfull(bounded_queue *queue) {
    return (queue->first == queue->tail_cpy) &&
        (queue->tail_cpy ==
         atomic_load_explicit(&queue->contail, memory_order_relaxed));
}

char isempty(bounded_queue *queue) {
    return atomic_load_explicit(&queue->tail->next, memory_order_relaxed);
}

char push(bounded_queue *into, void *topush) {
    node *n = alloc_node(into);
    if (!n)
        return 0;
    n->next = 0;
    n->data = topush;
    atomic_store_explicit(&from->head->next, n, memory_order_release);
    from->head = n;
}

char pop(bounded_queue *from, void **elem) {
    if (atomic_load_explicit(&from->contail->next, memory_order_aquire)) {
        *elem = from->contail->next->data;
        atomic_store_explicit(&from->contail, from->contail->next, memory_order_release);
        return 1;
    }
    return 0;
}
