#include "tracked_free_list.h"
#include "assert.h"
#define EMPTY_KEEP 2

#define GET_BLOB_DATA(x) (x)
#define SET_NEXT_BLOB(x, newb) *((void **)x) = newb;
#define GET_NEXT_BLOB(x) (*((void **)x))
#define SET_BLOCK(x, data_size, inb)  *(void **)((char *)(x) + data_size - sizeof(short)) = inb;
#define GET_BLOCK(x, data_size) (*(void **)((char *)(x) + data_size - sizeof(void *)))
#define GET_SLAB(blk) ((slab *)((char *)(blk) - slab_extra))
#define FIRST(x) (((block *)((char *)(x) + slab_extra))->next)

#define slab_extra sizeof(struct {size_t pad_64; size_t s; void *v; void *q;})
typedef struct slab {
    size_t num_alloc;
    struct slab *next, *prev;
    size_t pad64;
    block datablk;
} slab;

static void remove_slab (slab **fromp, slab *inslab) {
    if (inslab->next == inslab) {
        inslab->next = inslab->prev = NULL;
        *fromp = NULL;
    }
    inslab->next->prev = inslab->prev;
    inslab->prev->next = inslab->next;
    *fromp = (inslab == *fromp) ? inslab->next : *fromp;
}

static void add_slab_tail(slab **listp, slab *newslab) {
    slab *list = *listp;
    if (!list) {
        newslab->next = newslab->prev = newslab;
        *listp = newslab;
    }
    else {
        newslab->next = list;
        newslab->prev = list->prev;
        if (list->prev)
            list->prev->next = newslab;
        list->prev = newslab;
    }
}

static void add_slab_head(slab **listp, slab *newslab) {
    slab *list = *listp;
    if (!list) {
        newslab->next = newslab->prev = newslab;
        *listp = newslab;
    }
    else {
        newslab->next = list->next;
        newslab->prev = list;
        if (list->next)
            list->next->prev = newslab;
        list->next = newslab;
        *listp = newslab;
    }
}


static void init_slab_list(void *slabdat, size_t num_elem, size_t elem_size, void *slab) {
    void *cur_blob = slabdat;
    for(size_t i = 0; i < num_elem - 1; i++) {
        SET_NEXT_BLOB(cur_blob, ((char *)cur_blob + elem_size));
        SET_BLOCK(cur_blob, elem_size, slab)
        cur_blob = (char *)cur_blob + elem_size;
    }
    SET_NEXT_BLOB(cur_blob, NULL);
    SET_BLOCK(cur_blob, elem_size, slab);
}

static void *add_slab_to(tracked_free_list *into) {
    block *curblock = get_block(into->cache);
    if (!curblock)
        return NULL;

    init_slab_list((char *)curblock->data + into->data_size,
                   into->num_per_slab - 1,
                   into->data_size,
                   curblock);
    GET_SLAB(curblock)->num_alloc = 1;
    SET_BLOCK(curblock->data, into->data_size, curblock);
    add_slab_head(&into->partial, GET_SLAB(curblock));
    return curblock->data;
}

static inline void *_alloc(tracked_free_list *from) {
    void *first_open = FIRST(from->partial);
    from->partial->num_alloc++;
    void *data = GET_BLOB_DATA(first_open);
    void *next = GET_NEXT_BLOB(first_open);
    FIRST(from->partial) = next;
    if (!next) {
        slab *fulls = from->partial;
        remove_slab(&from->partial, from->partial);
        add_slab_tail(&from->full, fulls);
    }
    return data;
}

//could do some slight optimizations in this and move full
//namely eliminate a branch/2 and a few instrs and add/remove slab.
//But I don't expect these fncs to be called often enough for it
//to seriously make a difference

static void *add_partial(tracked_free_list *into) {
    if (into->empty) {
        slab *emptys = into->empty;
        remove_slab(&into->empty, into->empty);
        add_slab_head(&into->partial, emptys);
        return _alloc(into);
    }
    return add_slab_to(into);
}

void *tracked_alloc(tracked_free_list *from) {
    if (!from->partial)
        return add_partial(from);
    return _alloc(from);
}

static void del_slab(block_cache *cache, slab *todel) {
    return_block(cache, &todel->datablk);
}

static void proc_empty(tracked_free_list *from, slab *nempty) {
    remove_slab(&from->partial, nempty);
    if (from->num_empty > EMPTY_KEEP)
        del_slab(from->cache, nempty);
    else
        add_slab_head(&from->empty, nempty);
}

void tracked_free(tracked_free_list *from, void *data) {
    if (!data)
        return;
    block *curblock = GET_BLOCK(data, from->data_size);
    slab *cslab = GET_SLAB(curblock);
    cslab->num_alloc--;
    char isfull = !curblock->next;
    SET_NEXT_BLOB(data, curblock->next);

    if (isfull) {
        remove_slab (&from->full, cslab);
        add_slab_tail(&from->partial, cslab);
        return;
    }
    else if (!cslab->num_alloc) {
        proc_empty(from, cslab);
    }
}

typedef struct chunk {void *data[2];} chunk;

static uint16_t get_data_size(uint16_t in) {
    if (in < sizeof(chunk))
        return sizeof (chunk);
    if (!(in % sizeof(chunk)))
        return in;
    return in + sizeof(chunk) - (in % sizeof(chunk));
}

void init_tracked_list(tracked_free_list *toinit, uint16_t alloc_size, block_sizes which) {
    toinit->which = which;
    toinit->partial = toinit->full = toinit->empty = NULL;
    toinit->data_size = get_data_size(alloc_size);
    toinit->num_per_slab = which - sizeof(slab);
    toinit->cache = get_thread_local_cache((which));
    assert(toinit->num_per_slab >= 2);
}

static void clear_a_list(block_cache *cache, slab *lst) {
    if (!lst)
        return;
    lst->prev->next = NULL;
    while (lst) {
        slab *toclear = lst;
        lst = lst->next;
        return_block(cache, &toclear->datablk);
    }
}

void clear_tracked_list(tracked_free_list *toclear) {
    clear_a_list(toclear->cache, toclear->empty);
    clear_a_list(toclear->cache, toclear->full);
    clear_a_list(toclear->cache, toclear->partial);
    toclear->partial = toclear->full = toclear->empty = NULL;
    toclear->num_empty = 0;
}
