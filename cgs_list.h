#include "cgs_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* Common macros (include only once) */
#ifndef CGS_LIST_H
#define CGS_LIST_H
#define cgs_list_foreach(t, l, n, e) for (CGS_CAT(t, node) *n = CGS_CAT(t, front_node)(l), *n##__n = n->next; n; n = NULL) \
                                         for (CGS_CAT(t, type) e = n->dat; n != &(l)->root; n = n##__n, n##__n = n->next, e = n->dat)
#define cgs_list_foreach_r(t, l, n, e) for (CGS_CAT(t, node) *n = CGS_CAT(t, back_node)(l), *n##__n = n->prev; n; n = NULL) \
                                           for (CGS_CAT(t, type) e = n->dat; n != &(l)->root; n = n##__n, n##__n = n->prev, e = n->dat)
#endif

/* semi include guard */
#if !CGS_CAT(cgs, cgs_name)

typedef cgs_type CGS_FUNCTION(type);

typedef struct CGS_FUNCTION(node) {
    cgs_type dat;
    struct CGS_FUNCTION(node) *prev, *next;
} CGS_FUNCTION(node);

typedef struct {
    size_t size;
    CGS_FUNCTION(node) root;
} cgs_name;

static inline cgs_name *CGS_FUNCTION(new)() {
    cgs_name *res = malloc(sizeof(cgs_name));
    res->size = 0;
    res->root.prev = res->root.next = &res->root;
    return res;
}

static inline bool CGS_FUNCTION(empty)(cgs_name *l) {
    return l->size == 0;
}

static inline CGS_FUNCTION(node) *CGS_FUNCTION(insert_after)(cgs_name *l, CGS_FUNCTION(node) *n, cgs_type e) {
    CGS_FUNCTION(node) *node = malloc(sizeof(CGS_FUNCTION(node)));
    node->dat = e;

    node->next = n->next;
    n->next->prev = node;

    node->prev = n;
    n->next = node;

    l->size++;
    return node;
}

static inline CGS_FUNCTION(node) *CGS_FUNCTION(insert_before)(cgs_name *l, CGS_FUNCTION(node) *n, cgs_type e) {
    CGS_FUNCTION(node) *node = malloc(sizeof(CGS_FUNCTION(node)));
    node->dat = e;

    node->prev = n->prev;
    n->prev->next = node;

    node->next = n;
    n->prev = node;

    l->size++;
    return node;
}

static inline void CGS_FUNCTION(erase)(cgs_name *l, CGS_FUNCTION(node) *n) {
    assert(l->size > 0);

    n->next->prev = n->prev;
    n->prev->next = n->next;
    free(n);

    l->size--;
}

static inline void CGS_FUNCTION(splice_after)(cgs_name *t, cgs_name *f, CGS_FUNCTION(node) *n) {
    if (f->size == 0) {
        return;
    }

    n->next->prev = f->root.prev;
    f->root.prev->next = n->next;

    n->next = f->root.next;
    f->root.next->prev = n;

    f->root.next = f->root.prev = &f->root;

    t->size += f->size;
    f->size = 0;
}

static inline void CGS_FUNCTION(splice_before)(cgs_name *t, cgs_name *f, CGS_FUNCTION(node) *n) {
    if (f->size == 0) {
        return;
    }

    n->prev->next = f->root.next;
    f->root.next->prev = n->prev;

    n->prev = f->root.prev;
    f->root.prev->next = n;

    f->root.next = f->root.prev = &f->root;

    t->size += f->size;
    f->size = 0;
}

static inline void CGS_FUNCTION(push_back)(cgs_name *l, cgs_type e) {
    CGS_FUNCTION(insert_before)(l, &l->root, e);
}

static inline cgs_type CGS_FUNCTION(pop_back)(cgs_name *l) {
    cgs_type res = l->root.prev->dat;
    CGS_FUNCTION(erase)(l, l->root.prev);
    return res;
}

static inline void CGS_FUNCTION(push_front)(cgs_name *l, cgs_type e) {
    CGS_FUNCTION(insert_after)(l, &l->root, e);
}

static inline cgs_type CGS_FUNCTION(pop_front)(cgs_name *l) {
    cgs_type res = l->root.next->dat;
    CGS_FUNCTION(erase)(l, l->root.next);
    return res;
}

static inline CGS_FUNCTION(node) *CGS_FUNCTION(front_node)(cgs_name *l) {
    return l->root.next;
}

static inline CGS_FUNCTION(node) *CGS_FUNCTION(back_node)(cgs_name *l) {
    return l->root.prev;
}

static inline CGS_FUNCTION(node) *CGS_FUNCTION(sentinel_node)(cgs_name *l) {
    return &l->root;
}

static inline cgs_type CGS_FUNCTION(front)(cgs_name *l) {
    assert(l->size > 0);
    return l->root.next->dat;
}

static inline cgs_type CGS_FUNCTION(back)(cgs_name *l) {
    assert(l->size > 0);
    return l->root.prev->dat;
}

static inline void CGS_FUNCTION(clear)(cgs_name *l) {
    CGS_FUNCTION(node) *curr = l->root.next, *next;
    while (curr != &l->root) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    l->root.next = l->root.prev = &l->root;
    l->size = 0;
}

static inline void CGS_FUNCTION(free)(cgs_name *l) {
    CGS_FUNCTION(clear)(l);
    free(l);
}

#undef cgs_type
#undef cgs_name
#endif /* semi include guard */
