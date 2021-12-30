#include "cgs_common.h"
#include <stdlib.h>
#include <assert.h>

/* semi include guard */
#if !CGS_CAT2(cgs_, cgs_name)

#define CGS_VECTOR_INIT_CAPACITY 8

typedef struct {
    cgs_type *array;
    int size, capacity;
} cgs_name;

static inline cgs_name *CGS_FUNCTION(new)() {
    cgs_name * v = malloc(sizeof(cgs_name));
    v->size = 0;
    v->capacity = CGS_VECTOR_INIT_CAPACITY;
    v->array = malloc(sizeof(cgs_type) * CGS_VECTOR_INIT_CAPACITY);
    return v;
}

static inline cgs_type CGS_FUNCTION(at)(cgs_name *v, int index) {
    assert(index >= 0 && index < v->size);
    return v->array[index];
}

static inline void CGS_FUNCTION(push_back)(cgs_name *v, cgs_type e) {
    if (v->size >= v->capacity) {
        v->capacity *= 2;
        v->array = realloc(v->array, sizeof(cgs_type) * v->capacity);
    }
    v->array[v->size++] = e;
}

static inline void CGS_FUNCTION(free)(cgs_name *v) {
    free(v->array);
    free(v);
}

#include "cgs_reset.h"
#endif /* include guard */
