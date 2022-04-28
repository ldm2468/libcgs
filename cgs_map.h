#include "cgs_common.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* Common functions (include only once) */
#ifndef CGS_MAP_H
#define CGS_MAP_H

static inline uint32_t cgs_map_hash_single(uint32_t x) {
    // https://github.com/skeeto/hash-prospector
    x ^= x >> 15;
    x *= 0xd168aaad;
    x ^= x >> 15;
    x *= 0xaf723597;
    x ^= x >> 15;
    return x;
}

static inline uint32_t cgs_map_hash(const void *ptr, size_t size) {
    uint32_t res = 1;
    while (size & 3) { // size % 4
        res <<= 8;
        res |= ((char *) ptr)[size - 1];
        size--;
    }
    res = cgs_map_hash_single(res);
    for (size_t i = 0; i < size; i += 4) {
        res ^= ((uint32_t *) ptr)[i];
        res = cgs_map_hash_single(res);
    }
    return res;
}

static inline uint32_t cgs_map_hash_str(const char *ptr) {
    uint32_t res = 1;
    while(ptr[0] != 0) {
        uint32_t next = 1;
        for (int i = 0; i < 4 && ptr[0] != 0; i++) { // size % 4
            next <<= 8;
            next |= ptr[0];
            ptr++;
        }
        res ^= next;
        res = cgs_map_hash_single(res);
    }
    return res;
}

#endif

/* semi include guard */
#if !CGS_CAT(cgs, cgs_name)

typedef cgs_key CGS_FUNCTION(key);
typedef cgs_value CGS_FUNCTION(value);

#ifdef cgs_map_default_hash
static inline uint32_t CGS_FUNCTION(hash)(cgs_key k) {
    return cgs_map_hash(&k, sizeof(cgs_key));
}
#endif

#ifdef cgs_map_default_hash_str
static inline uint32_t CGS_FUNCTION(hash)(cgs_key k) {
    return cgs_map_hash_str(k);
}
#endif

typedef struct {
    // todo
} cgs_name;

typedef struct {
    cgs_key key;
    uint32_t hash;
    cgs_value value;
} CGS_FUNCTION(entry);

#undef cgs_key
#undef cgs_value
#undef cgs_name
#undef cgs_map_default_hash
#undef cgs_map_default_hash_str
#endif /* include guard */
