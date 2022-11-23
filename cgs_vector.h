#include "cgs_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* Common macros (include only once) */
#ifndef CGS_VECTOR_H
#define CGS_VECTOR_H

#define CGS_VECTOR(name) CGS_CAT(cgs_vec_name, name)

#endif

/* semi include guard */
#if !CGS_CAT(cgs, cgs_vec_name)

typedef cgs_vec_type CGS_VECTOR(type);

#define CGS_VECTOR_INIT_CAPACITY 8

typedef struct {
    cgs_vec_type *array;
    size_t size, capacity;
} cgs_vec_name;

/**
 * @brief Allocate and initialize a new vector.
 * @return A newly allocated and initialized vector.
 */
static inline cgs_vec_name *CGS_VECTOR(new)() {
    cgs_vec_name *v = malloc(sizeof(cgs_vec_name));
    v->size = 0;
    v->capacity = CGS_VECTOR_INIT_CAPACITY;
    v->array = malloc(sizeof(cgs_vec_type) * CGS_VECTOR_INIT_CAPACITY);
    return v;
}

/**
 * @brief Get the element at a given index in the vector.
 * @param v The vector to query.
 * @param index The index to the desired element.
 * @return The element at the given index.
 */
static inline cgs_vec_type CGS_VECTOR(at)(cgs_vec_name *v, size_t index) {
    assert(index < v->size);
    return v->array[index];
}

/**
 * @brief Set the element at a given index in the vector.
 * @param v The vector to query.
 * @param index The index to the desired element.
 * @param e The element to set to.
 */
static inline void CGS_VECTOR(set)(cgs_vec_name *v, size_t index, cgs_vec_type e) {
    assert(index < v->size);
    v->array[index] = e;
}

/**
 * @brief Check whether the vector is empty.
 * @param v The vector to query.
 * @return Whether the vector is empty.
 */
static inline bool CGS_VECTOR(empty)(cgs_vec_name *v) {
    return v->size == 0;
}

/**
 * @brief Reserve capacity in the vector.
 * Ensures that the vector's capacity is at least as large as the given size.
 * @param v The vector to use.
 * @param s The desired capacity.
 */
static inline void CGS_VECTOR(reserve)(cgs_vec_name *v, size_t s) {
    if (s > v->capacity) {
        v->capacity = s > v->capacity * 2 ? s : v->capacity * 2;
        v->array = realloc(v->array, sizeof(cgs_vec_type) * v->capacity);
    }
}

/**
 * @brief Push an element to the end of the vector.
 * @param v The vector to use.
 * @param e The element to push.
 */
static inline void CGS_VECTOR(push_back)(cgs_vec_name *v, cgs_vec_type e) {
    CGS_VECTOR(reserve)(v, v->size + 1);
    v->array[v->size++] = e;
}

/**
 * @brief Pop an element from the end of the vector and return it.
 * @param v The vector to use.
 * @return The element that was popped.
 */
static inline cgs_vec_type CGS_VECTOR(pop_back)(cgs_vec_name *v) {
    assert(v->size > 0);
    return v->array[--v->size];
}

/**
 * @brief Insert an element at a given position in the vector.
 * @param v The vector to use.
 * @param pos The position to insert the element.
 * @param e The element to insert.
 */
static inline void CGS_VECTOR(insert)(cgs_vec_name *v, size_t pos, cgs_vec_type e) {
    assert(pos <= v->size);
    CGS_VECTOR(reserve)(v, v->size + 1);
    memmove(&v->array[pos + 1], &v->array[pos], (v->size - pos) * sizeof(cgs_vec_type));
    v->array[pos] = e;
    v->size++;
}

/**
 * @brief Remove an element at a given position in the vector and return it.
 * @param v The vector to use.
 * @param pos The position of the element to remove.
 * @return The removed element.
 */
static inline cgs_vec_type CGS_VECTOR(erase)(cgs_vec_name *v, size_t pos) {
    cgs_vec_type res;
    assert(pos < v->size);
    res = v->array[pos];
    v->size--;
    memmove(&v->array[pos], &v->array[pos + 1], (v->size - pos) * sizeof(cgs_vec_type));
    return res;
}

/**
 * @brief Finds the index of the first element identical to e.
 * Returns -1 if there are no matches.
 * @param l The vector to query.
 * @param e The element to find.
 * @return The index of the first element identical to e, or -1 if there are no matches.
 */
static inline size_t CGS_VECTOR(find)(cgs_vec_name *v, cgs_vec_type e) {
    for (size_t i = 0; i < v->size; i++) {
        if (v->array[i] == e) {
            return i;
        }
    }
    return -1;
}

/**
 * @brief Remove all elements from the vector.
 * @param v The vector to use.
 */
static inline void CGS_VECTOR(clear)(cgs_vec_name *v) {
    v->size = 0;
}

/**
 * @brief Frees the vector and all of its data structures.
 * @param v The vector to free.
 */
static inline void CGS_VECTOR(free)(cgs_vec_name *v) {
    free(v->array);
    free(v);
}

#undef cgs_vec_type
#undef cgs_vec_name
#endif /* include guard */
