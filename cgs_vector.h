#include "cgs_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* semi include guard */
#if !CGS_CAT(cgs, cgs_name)

typedef cgs_type CGS_FUNCTION(type);

#define CGS_VECTOR_INIT_CAPACITY 8

typedef struct {
    cgs_type *array;
    size_t size, capacity;
} cgs_name;

/**
 * @brief Allocate and initialize a new vector.
 * @return A newly allocated and initialized vector.
 */
static inline cgs_name *CGS_FUNCTION(new)() {
    cgs_name *v = malloc(sizeof(cgs_name));
    v->size = 0;
    v->capacity = CGS_VECTOR_INIT_CAPACITY;
    v->array = malloc(sizeof(cgs_type) * CGS_VECTOR_INIT_CAPACITY);
    return v;
}

/**
 * @brief Get the element at a given index in the vector.
 * @param v The vector to query.
 * @param index The index to the desired element.
 * @return The element at the given index.
 */
static inline cgs_type CGS_FUNCTION(at)(cgs_name *v, size_t index) {
    assert(index < v->size);
    return v->array[index];
}

/**
 * @brief Check whether the vector is empty.
 * @param v The vector to query.
 * @return Whether the vector is empty.
 */
static inline bool CGS_FUNCTION(empty)(cgs_name *v) {
    return v->size == 0;
}

/**
 * @brief Reserve capacity in the vector.
 * Ensures that the vector's capacity is at least as large as the given size.
 * @param v The vector to use.
 * @param s The desired capacity.
 */
static inline void CGS_FUNCTION(reserve)(cgs_name *v, size_t s) {
    if (s > v->capacity) {
        v->capacity = s > v->capacity * 2 ? s : v->capacity * 2;
        v->array = realloc(v->array, sizeof(cgs_type) * v->capacity);
    }
}

/**
 * @brief Push an element to the end of the vector.
 * @param v The vector to use.
 * @param e The element to push.
 */
static inline void CGS_FUNCTION(push_back)(cgs_name *v, cgs_type e) {
    CGS_FUNCTION(reserve)(v, v->size + 1);
    v->array[v->size++] = e;
}

/**
 * @brief Pop an element from the end of the vector and return it.
 * @param v The vector to use.
 * @return The element that was popped.
 */
static inline cgs_type CGS_FUNCTION(pop_back)(cgs_name *v) {
    assert(v->size > 0);
    return v->array[--v->size];
}

/**
 * @brief Insert an element at a given position in the vector.
 * @param v The vector to use.
 * @param pos The position to insert the element.
 * @param e The element to insert.
 */
static inline void CGS_FUNCTION(insert)(cgs_name *v, size_t pos, cgs_type e) {
    assert(pos <= v->size);
    CGS_FUNCTION(reserve)(v, v->size + 1);
    memmove(&v->array[pos + 1], &v->array[pos], (v->size - pos) * sizeof(cgs_type));
    v->array[pos] = e;
    v->size++;
}

/**
 * @brief Remove an element at a given position in the vector and return it.
 * @param v The vector to use.
 * @param pos The position of the element to remove.
 * @return The removed element.
 */
static inline cgs_type CGS_FUNCTION(erase)(cgs_name *v, size_t pos) {
    cgs_type res;
    assert(pos < v->size);
    res = v->array[pos];
    v->size--;
    memmove(&v->array[pos], &v->array[pos + 1], (v->size - pos) * sizeof(cgs_type));
    return res;
}

/**
 * @brief Remove all elements from the vector.
 * @param v The vector to use.
 */
static inline void CGS_FUNCTION(clear)(cgs_name *v) {
    v->size = 0;
}

/**
 * @brief Frees the vector and all of its data structures.
 * @param v The vector to free.
 */
static inline void CGS_FUNCTION(free)(cgs_name *v) {
    free(v->array);
    free(v);
}

#undef cgs_type
#undef cgs_name
#endif /* include guard */
