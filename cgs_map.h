/**
 * An unordered map, implemented with a hash table using linear hashing.
 *
 * Define the following macros before including the header to customize the map.
 * - cgs_map_name: Required. The name of the generated map type. (e.g. `my_map`)
 * - cgs_map_key: Required. The type of the key. (e.g. `int`, `char *`)
 * - cgs_map_value: Required. The type of the value. (e.g. `int`, `char *`)
 * - cgs_map_initial_capacity: Optional. The initial capacity of the map. (Default: 16)
 * - cgs_map_default_value: Optional. The default value returned when the element is not found. (Default: 0)
 * - cgs_map_load_factor: Optional. The target load factor as an integer percentage. (Default: 75)
 *
 * The following three macros define the hashing function used. Only one must be defined.
 * - cgs_map_default_hash: The default hash function, suitable for basic key types like `int` or `long`.
 * - cgs_map_default_hash_str: The default hash function for null-terminated strings.
 * - cgs_map_default_hash_ptr: The default hash function for pointers to data with a fixed size.
 *
 * The default hashing functions do not support pointers to variable length data.
 * Instead, the hashing function must be manually defined with the following signature prior to including
 * this header. Replace `<cgs_map_name>` with the defined map name.
 * ```static inline uint32_t <cgs_map_name>_hash(cgs_map_key k)```
 */

#include "cgs_common.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/* Common functions (include only once) */
#ifndef CGS_MAP_H
#define CGS_MAP_H

/**
 * @brief Hash a single 32-bit integer.
 * The algorithm from https://github.com/skeeto/hash-prospector is used.
 * @param x The integer to hash.
 * @return The hash result.
 */
static inline uint32_t cgs_map_hash_single(uint32_t x) {
    // https://github.com/skeeto/hash-prospector
    x ^= x >> 15;
    x *= 0xd168aaad;
    x ^= x >> 15;
    x *= 0xaf723597;
    x ^= x >> 15;
    return x;
}

/**
 * @brief Hash the given data.
 * @param ptr The pointer to the data.
 * @param size The size of the data.
 * @return The hash result.
 */
static inline uint32_t cgs_map_hash(const void *ptr, size_t size) {
    uint32_t res = 1;
    while (size & 3) { // size % 4
        res <<= 8;
        res |= ((char *) ptr)[size - 1];
        size--;
    }
    res = cgs_map_hash_single(res);
    for (size_t i = 0; i < size / 4; i++) {
        res ^= ((uint32_t *) ptr)[i];
        res = cgs_map_hash_single(res);
    }
    return res;
}

/**
 * @brief Hash the given string.
 * @param ptr The string to hash.
 * @return The hash result.
 */
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

#define CGS_MAP(name) CGS_CAT(cgs_map_name, name)
#define CGS_MAP_INTERNAL(name) CGS_CAT_INTERNAL(cgs_map_name, name)

#endif

/* semi include guard */
#if !CGS_CAT(cgs, cgs_map_name)

typedef cgs_map_key CGS_MAP(key);
typedef cgs_map_value CGS_MAP(value);

#ifndef cgs_map_initial_capacity
#define cgs_map_initial_capacity 16
#endif

#ifndef cgs_map_default_value
#define cgs_map_default_value 0
#endif

#ifndef cgs_map_load_factor
#define cgs_map_load_factor 75
#endif

typedef struct CGS_MAP(entry) {
    cgs_map_key key;
    uint32_t hash;
    cgs_map_value value;
    struct CGS_MAP(entry) *next_in_bucket, *prev_in_bucket;
    struct CGS_MAP(entry) *next, *prev;
} CGS_MAP(entry);

#define cgs_vec_type CGS_MAP(entry) *
#define cgs_vec_name CGS_MAP_INTERNAL(vec)
#include "cgs_vector.h"

#ifdef cgs_map_default_hash_str
static inline uint32_t CGS_MAP(hash)(cgs_map_key k) {
    return cgs_map_hash_str(k);
}
#undef cgs_map_default_hash_str
#endif

#ifdef cgs_map_default_hash_ptr
static inline uint32_t CGS_MAP(hash)(cgs_map_key k) {
    return cgs_map_hash(k, sizeof(*k));
}
#undef cgs_map_default_hash_ptr
#endif

#ifdef cgs_map_default_hash
static inline uint32_t CGS_MAP(hash)(cgs_map_key k) {
    return cgs_map_hash(&k, sizeof(cgs_map_key));
}
#undef cgs_map_default_hash
#endif

typedef struct {
    size_t size;
    size_t hash_base;
    size_t split_index;
    CGS_MAP_INTERNAL(vec) *vec;
    CGS_MAP(entry) root;
} cgs_map_name;

/**
 * @brief Allocates and initializes a new map.
 * @return A newly allocated and initialized map.
 */
static inline cgs_map_name *CGS_MAP(new)() {
    cgs_map_name *m = malloc(sizeof(cgs_map_name));
    m->vec = CGS_MAP_INTERNAL(vec_new)();
    CGS_MAP_INTERNAL(vec_reserve)(m->vec, cgs_map_initial_capacity);
    for (size_t i = 0; i < cgs_map_initial_capacity; i++) {
        CGS_MAP_INTERNAL(vec_push_back)(m->vec, NULL);
    }
    m->root.next = m->root.prev = &m->root;

    m->hash_base = cgs_map_initial_capacity;
    m->size = 0;
    m->split_index = 0;
    return m;
}

/* Maps the 32-bit hash to the length of the map's backing vector. */
static inline size_t CGS_MAP_INTERNAL(normalize_hash)(cgs_map_name *m, uint32_t hash) {
    size_t low_hash = hash & (m->hash_base - 1);
    if (low_hash < m->split_index) {
        low_hash = hash & (m->hash_base * 2 - 1);
    }
    return low_hash;
}
/* Finds the entry with the given key in a bucket. */
static inline CGS_MAP(entry) *CGS_MAP_INTERNAL(find_entry)(cgs_map_name *m, size_t low_hash, cgs_map_key key) {
    CGS_MAP(entry) *entry = CGS_MAP_INTERNAL(vec_at)(m->vec, low_hash);
    while (entry != NULL && entry->key != key) {
        entry = entry->next_in_bucket;
    }
    return entry;
}

/* Inserts an entry to the global list. */
static inline void CGS_MAP_INTERNAL(insert_entry)(cgs_map_name *m, CGS_MAP(entry) *entry) {
    entry->prev = m->root.prev;
    entry->next = &m->root;
    entry->prev->next = entry->next->prev = entry;
}

/* Splits a bucket with linear hashing. */
static inline void CGS_MAP_INTERNAL(split)(cgs_map_name *m) {
    CGS_MAP_INTERNAL(vec_push_back)(m->vec, NULL);

    /* the entry to split */
    CGS_MAP(entry) *entry = CGS_MAP_INTERNAL(vec_at)(m->vec, m->split_index);
    CGS_MAP(entry) *prev_in_old_bucket = NULL;

    while (entry != NULL) {
        CGS_MAP(entry) *next_in_bucket = entry->next_in_bucket;
        uint32_t new_hash = entry->hash & (m->hash_base * 2 - 1);
        if (new_hash != m->split_index) { /* move to new bucket */
            entry->next_in_bucket = CGS_MAP_INTERNAL(vec_at)(m->vec, new_hash);
            CGS_MAP_INTERNAL(vec_set)(m->vec, new_hash, entry);

            /* remove from old bucket */
            if (prev_in_old_bucket == NULL) {
                CGS_MAP_INTERNAL(vec_set)(m->vec, m->split_index, next_in_bucket);
            } else {
                prev_in_old_bucket->next_in_bucket = next_in_bucket;
            }
            entry = prev_in_old_bucket; /* since this entry is no longer in the old bucket, skip it */
        }
        prev_in_old_bucket = entry;
        entry = next_in_bucket;
    }
    m->split_index++;

    if (m->split_index >= m->hash_base) {
        m->split_index = 0;
        m->hash_base *= 2;
    }
}

/**
 * @brief Inserts a key-value pair to the map.
 * If the key already exists, the existing value is modified.
 * @param m The map to use.
 * @param key The key to insert.
 * @param value The value to insert.
 */
static inline void CGS_MAP(insert)(cgs_map_name *m, cgs_map_key key, cgs_map_value value) {
    uint32_t hash = CGS_MAP(hash)(key);
    size_t low_hash = CGS_MAP_INTERNAL(normalize_hash)(m, hash);

    /* check if key already exists */
    CGS_MAP(entry) *old_entry = CGS_MAP_INTERNAL(find_entry)(m, low_hash, key);
    if (old_entry != NULL) {
        old_entry->value = value;
        return;
    }

    CGS_MAP(entry) *new_entry = malloc(sizeof(CGS_MAP(entry)));
    CGS_MAP_INTERNAL(insert_entry)(m, new_entry);

    new_entry->next_in_bucket = CGS_MAP_INTERNAL(vec_at)(m->vec, low_hash);
    CGS_MAP_INTERNAL(vec_set)(m->vec, low_hash, new_entry);

    new_entry->hash = hash;
    new_entry->key = key;
    new_entry->value = value;

    m->size++;
    while (m->vec->size < m->size * 100 / cgs_map_load_factor) {
        CGS_MAP_INTERNAL(split)(m);
    }
}

/**
 * @brief Finds the value associated with the given key.
 * If the key is not found, returns the default value defined with `cgs_map_default_value`.
 * @param m The map to use.
 * @param key The key to find.
 * @return The value associated with the given key, or the default value.
 */
static inline cgs_map_value CGS_MAP(find)(cgs_map_name *m, cgs_map_key key) {
    uint32_t hash = CGS_MAP(hash)(key);
    size_t low_hash = CGS_MAP_INTERNAL(normalize_hash)(m, hash);
    CGS_MAP(entry) *entry = CGS_MAP_INTERNAL(find_entry)(m, low_hash, key);
    return entry == NULL ? (cgs_map_default_value) : entry->value;
}

/**
 * @brief Erases the entry with the given key and returns the value it was associated with.
 * If the key is not found, returns the default value defined with `cgs_map_default_value`.
 * @param m The map to use.
 * @param key The key to erase.
 * @return The value previously associated with the given key, or the default value.
 */
static inline cgs_map_value CGS_MAP(erase)(cgs_map_name *m, cgs_map_key key) {
    uint32_t hash = CGS_MAP(hash)(key);
    size_t low_hash = CGS_MAP_INTERNAL(normalize_hash)(m, hash);

    CGS_MAP(entry) *entry = CGS_MAP_INTERNAL(vec_at)(m->vec, low_hash), *prev = NULL;
    while (entry != NULL) {
        if (entry->key == key) {
            cgs_map_value res = entry->value;
            entry->prev->next = entry->next;
            entry->next->prev = entry->prev;

            if (prev == NULL) {
                CGS_MAP_INTERNAL(vec_set)(m->vec, low_hash, entry->next_in_bucket);
            } else {
                prev->next_in_bucket = entry->next_in_bucket;
            }

            free(entry);
            m->size--;
            return res;
        }
        prev = entry;
        entry = entry->next_in_bucket;
    }
    return cgs_map_default_value;
}

/**
 * @brief Removes all entries from the map.
 * @param m The map to use.
 */
static inline void CGS_MAP(clear)(cgs_map_name *m) {
    CGS_MAP(entry) *node = m->root.next, *next;
    while (node != &m->root) {
        next = node->next;
        free(node);
        node = next;
    }

    m->size = 0;
}

/**
 * @brief Frees the map and all of its data structures.
 * @param m The map to free.
 */
static inline void CGS_MAP(free)(cgs_map_name *m) {
    CGS_MAP(clear)(m);
    CGS_MAP_INTERNAL(vec_free)(m->vec);
    free(m);
}

#undef cgs_map_key
#undef cgs_map_value
#undef cgs_map_name
#undef cgs_map_default_value
#undef cgs_map_initial_capacity
#undef cgs_map_load_factor
#endif /* include guard */
