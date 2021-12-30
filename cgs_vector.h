#include "cgs_common.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* semi include guard */
#if !CGS_CAT2(cgs_, cgs_name)

#define CGS_VECTOR_INIT_CAPACITY 8

typedef struct {
    cgs_type *array;
    size_t size, capacity;
} cgs_name;

static inline cgs_name *CGS_FUNCTION(new)() {
    cgs_name *v = malloc(sizeof(cgs_name));
    v->size = 0;
    v->capacity = CGS_VECTOR_INIT_CAPACITY;
    v->array = malloc(sizeof(cgs_type) * CGS_VECTOR_INIT_CAPACITY);
    return v;
}

static inline cgs_type CGS_FUNCTION(at)(cgs_name *v, size_t index) {
    assert(index < v->size);
    return v->array[index];
}

static inline bool CGS_FUNCTION(empty)(cgs_name *v) {
    return v->size == 0;
}

static inline void CGS_FUNCTION(reserve)(cgs_name *v, size_t s) {
    if (s > v->capacity) {
        v->capacity = s > v->capacity * 2 ? s : v->capacity * 2;
        v->array = realloc(v->array, sizeof(cgs_type) * v->capacity);
    }
}

static inline void CGS_FUNCTION(push_back)(cgs_name *v, cgs_type e) {
    CGS_FUNCTION(reserve)(v, v->size + 1);
    v->array[v->size++] = e;
}

static inline cgs_type CGS_FUNCTION(pop_back)(cgs_name *v) {
    assert(v->size > 0);
    return v->array[--v->size];
}

static inline void CGS_FUNCTION(insert)(cgs_name *v, size_t pos, cgs_type e) {
    assert(pos <= v->size);
    CGS_FUNCTION(reserve)(v, v->size + 1);
    memmove(&v->array[pos + 1], &v->array[pos], (v->size - pos) * sizeof(cgs_type));
    v->array[pos] = e;
    v->size++;
}

static inline cgs_type CGS_FUNCTION(erase)(cgs_name *v, size_t pos) {
    cgs_type res;
    assert(pos < v->size);
    res = v->array[pos];
    v->size--;
    memmove(&v->array[pos], &v->array[pos + 1], (v->size - pos) * sizeof(cgs_type));
    return res;
}

static inline void CGS_FUNCTION(clear)(cgs_name *v) {
    v->size = 0;
}

static inline void CGS_FUNCTION(free)(cgs_name *v) {
    free(v->array);
    free(v);
}

#undef cgs_type
#undef cgs_name
#endif /* include guard */
