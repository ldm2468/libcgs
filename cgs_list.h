/**
 * @file cgs_list.h
 * @brief A linked list with basic iteration macros.
 *
 * Define the following macros before including the header.
 * - cgs_list_name: The name of the generated list type. (e.g. `my_list`)
 * - cgs_list_type: The type of the elements. (e.g. `int`, `char *`)
 *
 * After the header is included, define the macro `cgs_<cgs_list_name>` to 1.
 * This is to prevent clashes from multiple includes.
 *
 * For example, the following code generates the type `dlist` as a list of doubles.
 * ```
 * #define cgs_list_type double
 * #define cgs_list_name dlist
 * #include "cgs_list.h"
 * #define cgs_dlist 1
 * ```
 */

#include "cgs_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* Common macros (include only once) */
#ifndef CGS_LIST_H
#define CGS_LIST_H
/**
 * @brief Generates a for-each loop.
 *
 * The parameters `n` and `e` are defined inside the macro, so they should not be defined outside.
 * The node variable `n` may be useful for functions such as `XXX_erase()`.
 *
 * @param t The name of the list type. (e.g. `my_list`)
 * @param l The list to iterate on.
 * @param n The variable that holds the current node.
 * @param e The variable that holds the current element.
 */
#define cgs_list_foreach(t, l, n, e) for (CGS_CAT(t, node) *n = CGS_CAT(t, front_node)(l), *n##__n = n->next; n; n = NULL) \
                                         for (CGS_CAT(t, type) e = n->dat; n != &(l)->root; n = n##__n, n##__n = n->next, e = n->dat)
/**
 * @brief Generates a for-each loop that iterates in reverse.
 *
 * @param t The name of the list type. (e.g. `my_list`)
 * @param l The list to iterate on.
 * @param n The variable that holds the current node.
 * @param e The variable that holds the current element.
 */
#define cgs_list_foreach_r(t, l, n, e) for (CGS_CAT(t, node) *n = CGS_CAT(t, back_node)(l), *n##__n = n->prev; n; n = NULL) \
                                           for (CGS_CAT(t, type) e = n->dat; n != &(l)->root; n = n##__n, n##__n = n->prev, e = n->dat)
#define CGS_LIST(name) CGS_CAT(cgs_list_name, name)
#endif

/* semi include guard */
#if !CGS_CAT(cgs, cgs_list_name)

typedef cgs_list_type CGS_LIST(type);

/**
 * An internal node of the linked list implementation.
 * It can be used like a C++ iterator in some functions.
 */
typedef struct CGS_LIST(node) {
    cgs_list_type dat;
    struct CGS_LIST(node) *prev, *next;
} CGS_LIST(node);

typedef struct {
    size_t size;
    CGS_LIST(node) root;
} cgs_list_name;

/**
 * @brief Allocate and initialize a new list.
 * @return A newly allocated and initialized list.
 */
static inline cgs_list_name *CGS_LIST(new)() {
    cgs_list_name *res = malloc(sizeof(cgs_list_name));
    res->size = 0;
    res->root.prev = res->root.next = &res->root;
    return res;
}

/**
 * @brief Check whether the list is empty.
 * @param l The list to query.
 * @return Whether the list is empty.
 */
static inline bool CGS_LIST(empty)(cgs_list_name *l) {
    return l->size == 0;
}

/**
 * @brief Insert an element inside the list after the node n.
 * n must be a valid node of list l, no error checking is provided.
 * @param l The list to modify.
 * @param n The node to insert after.
 * @param e The element to insert.
 * @return The newly created node with the inserted element.
 */
static inline CGS_LIST(node) *CGS_LIST(insert_after)(cgs_list_name *l, CGS_LIST(node) *n, cgs_list_type e) {
    CGS_LIST(node) *node = malloc(sizeof(CGS_LIST(node)));
    node->dat = e;

    node->next = n->next;
    n->next->prev = node;

    node->prev = n;
    n->next = node;

    l->size++;
    return node;
}

/**
 * @brief  Insert an element inside the list before the node n.
 * n must be a valid node of list l, no error checking is provided.
 * @param l The list to modify.
 * @param n The node to insert before.
 * @param e The element to insert.
 * @return The newly created node with the inserted element.
 */
static inline CGS_LIST(node) *CGS_LIST(insert_before)(cgs_list_name *l, CGS_LIST(node) *n, cgs_list_type e) {
    CGS_LIST(node) *node = malloc(sizeof(CGS_LIST(node)));
    node->dat = e;

    node->prev = n->prev;
    n->prev->next = node;

    node->next = n;
    n->prev = node;

    l->size++;
    return node;
}

/**
 * Erase a node in the list.
 * @param l The list to modify.
 * @param n The node to erase.
 */
static inline void CGS_LIST(erase)(cgs_list_name *l, CGS_LIST(node) *n) {
    assert(l->size > 0);

    n->next->prev = n->prev;
    n->prev->next = n->next;
    free(n);

    l->size--;
}

/**
 * @brief Splice together two lists.
 * All the elements of list f are moved to the list t after the node pos.
 * After the operation, the list f becomes empty.
 * @param t The list to splice to.
 * @param pos The node to insert the contents of f after. It must be a node of list t.
 * @param f The list to splice from. It becomes empty after this function finishes.
 */
static inline void CGS_LIST(splice_after)(cgs_list_name *t, CGS_LIST(node) *pos, cgs_list_name *f) {
    if (f->size == 0) {
        return;
    }

    pos->next->prev = f->root.prev;
    f->root.prev->next = pos->next;

    pos->next = f->root.next;
    f->root.next->prev = pos;

    f->root.next = f->root.prev = &f->root;

    t->size += f->size;
    f->size = 0;
}

/**
 * @brief Splice together two lists.
 * All the elements of list f are moved to the list t before the node pos.
 * After the operation, the list f becomes empty.
 * @param t The list to splice to.
 * @param pos The node to insert the contents of f before. It must be a node of list t.
 * @param f The list to splice from. It becomes empty after this function finishes.
 */
static inline void CGS_LIST(splice_before)(cgs_list_name *t, CGS_LIST(node) *pos, cgs_list_name *f) {
    if (f->size == 0) {
        return;
    }

    pos->prev->next = f->root.next;
    f->root.next->prev = pos->prev;

    pos->prev = f->root.prev;
    f->root.prev->next = pos;

    f->root.next = f->root.prev = &f->root;

    t->size += f->size;
    f->size = 0;
}

/**
 * @brief Pushes an element to the end of the list.
 * @param l The list to modify.
 * @param e The element to push.
 */
static inline void CGS_LIST(push_back)(cgs_list_name *l, cgs_list_type e) {
    CGS_LIST(insert_before)(l, &l->root, e);
}

/**
 * @brief Pops an element from the end of the list and returns it.
 * @param l The list to modify.
 * @return The popped element at the end of the list.
 */
static inline cgs_list_type CGS_LIST(pop_back)(cgs_list_name *l) {
    cgs_list_type res = l->root.prev->dat;
    CGS_LIST(erase)(l, l->root.prev);
    return res;
}

/**
 * @brief Pushes an element to the front of the list.
 * @param l The list to modify.
 * @param e The element to push.
 */
static inline void CGS_LIST(push_front)(cgs_list_name *l, cgs_list_type e) {
    CGS_LIST(insert_after)(l, &l->root, e);
}

/**
 * @brief Pops an element from the front of the list and returns it.
 * @param l The list to modify.
 * @return The popped element at the front of the list.
 */
static inline cgs_list_type CGS_LIST(pop_front)(cgs_list_name *l) {
    cgs_list_type res = l->root.next->dat;
    CGS_LIST(erase)(l, l->root.next);
    return res;
}

/**
 * @brief Returns the node that corresponds to the first element of the list.
 * If the list is empty, returns sentinel_node().
 * @param l The list to query.
 * @return The node that represents the first element of the list.
 */
static inline CGS_LIST(node) *CGS_LIST(front_node)(cgs_list_name *l) {
    return l->root.next;
}

/**
 * @brief Returns the node that corresponds to the last element of the list.
 * If the list is empty, returns sentinel_node().
 * @param l The list to query.
 * @return The node that represents the last element of the list.
 */
static inline CGS_LIST(node) *CGS_LIST(back_node)(cgs_list_name *l) {
    return l->root.prev;
}

/**
 * @brief Returns a node that does not correspond to any value.
 * It represents the node "after" front_node(), or "before" back_node().
 * Useful for loop conditions.
 * @param l The list to query.
 * @return The node that represents the bounds of the list.
 */
static inline CGS_LIST(node) *CGS_LIST(sentinel_node)(cgs_list_name *l) {
    return &l->root;
}

/**
 * @brief Finds the node of the first element identical to e.
 * @param l The list to query.
 * @param e The element to find.
 * @return The node that represents the first element identical to e.
 */
static inline CGS_LIST(node) *CGS_LIST(find)(cgs_list_name *l, cgs_list_type e) {
    CGS_LIST(node) *n = l->root.next;
    while (n != &l->root && n->dat != e) {
        n = n->next;
    }
    return n;
}

/**
 * @brief Returns the first element in the list.
 * Equivalent to front_node()->dat.
 * @param l The list to query.
 * @return The first element of the list.
 */
static inline cgs_list_type CGS_LIST(front)(cgs_list_name *l) {
    assert(l->size > 0);
    return l->root.next->dat;
}

/**
 * @brief Returns the last element in the list.
 * Equivalent to back_node()->dat.
 * @param l The list to query.
 * @return The last element of the list.
 */
static inline cgs_list_type CGS_LIST(back)(cgs_list_name *l) {
    assert(l->size > 0);
    return l->root.prev->dat;
}

/**
 * @brief Removes all elements from the list.
 * @param l The list to clear.
 */
static inline void CGS_LIST(clear)(cgs_list_name *l) {
    CGS_LIST(node) *curr = l->root.next, *next;
    while (curr != &l->root) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    l->root.next = l->root.prev = &l->root;
    l->size = 0;
}

/**
 * @brief Frees the list and all of its data structures.
 * If the list is not empty, it is cleared first.
 * @param l The list to free.
 */
static inline void CGS_LIST(free)(cgs_list_name *l) {
    CGS_LIST(clear)(l);
    free(l);
}

#undef cgs_list_type
#undef cgs_list_name
#endif /* semi include guard */
