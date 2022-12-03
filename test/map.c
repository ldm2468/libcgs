#include <stdint.h>

#define cgs_map_key int
#define cgs_map_value int
#define cgs_map_load_factor 50
#define cgs_map_default_hash
#define cgs_map_name iimap
#include "cgs_map.h"
#define cgs_iimap 1

#define cgs_map_key char *
#define cgs_map_value int
#define cgs_map_default_hash_str
#define cgs_map_name simap
#include "cgs_map.h"
#define cgs_simap 1

#define cgs_map_key int64_t
#define cgs_map_value int64_t
#define cgs_map_name llmap
#define cgs_map_default_hash
#define cgs_map_default_value (-1)
#include "cgs_map.h"
#define cgs_llmap 1

#include "cnit/cnit_main.h"
#define TEST_COUNT 8192

int test_hash() {
#define HIST_SIZE 16
    int histogram[HIST_SIZE] = { 0 };
    for (int i = 0; i < TEST_COUNT; i++) {
        histogram[cgs_map_hash_single(i + 12345) % HIST_SIZE]++;
    }
    for (int i = 0; i < HIST_SIZE; i++) {
        CNIT_ASSERT(histogram[i] < 2 * TEST_COUNT / HIST_SIZE);
        CNIT_ASSERT(histogram[i] > TEST_COUNT / 2 / HIST_SIZE);
        histogram[i] = 0;
    }
    for (int i = 0; i < 100; i++) {
        for (int j = i + 1; j < 100; j++) {
            CNIT_ASSERT(cgs_map_hash_single(i) != cgs_map_hash_single(j));
        }
    }
    CNIT_ASSERT(cgs_map_hash_str("") != cgs_map_hash_str("h"));
    CNIT_ASSERT(cgs_map_hash_str("h") != cgs_map_hash_str("he"));
    CNIT_ASSERT(cgs_map_hash_str("he") != cgs_map_hash_str("hel"));
    CNIT_ASSERT(cgs_map_hash_str("hel") != cgs_map_hash_str("hell"));
    CNIT_ASSERT(cgs_map_hash_str("hell") != cgs_map_hash_str("hello"));
    CNIT_ASSERT(cgs_map_hash_str("hello") != cgs_map_hash_str("Hello"));
    return 0;
}

int test_map_insert() {
    iimap *map = iimap_new();
    for (int i = 0; i < TEST_COUNT; i++) {
        iimap_insert(map, i * i, i);
        CNIT_ASSERT(map->size == i + 1);
    }

    int square = 0;
    int sqrt = 0;
    int delta = 1;
    for (int i = 0; i < TEST_COUNT * TEST_COUNT; i++) {
        if (i == square) {
            CNIT_ASSERT(iimap_find(map, i) == sqrt);
            square += delta;
            delta += 2;
            sqrt++;
        } else {
            CNIT_ASSERT(iimap_find(map, i) == 0);
        }
    }
    iimap_free(map);
    return 0;
}

int test_map_erase() {
    llmap *map = llmap_new();
    for (int i = 0; i < TEST_COUNT * 5; i++) {
        llmap_insert(map, i * 6, i * 6 + 1);
        CNIT_ASSERT(llmap_find(map, i * 6) == i * 6 + 1);
    }

    for (int i = 0; i < TEST_COUNT * 2; i++) {
        CNIT_ASSERT(llmap_erase(map, i * 15) == (i % 2 == 0 ? i * 15 + 1 : -1));
        CNIT_ASSERT(map->size == TEST_COUNT * 5 - (i / 2) - 1);
    }

    for (int i = 0; i < TEST_COUNT * 30; i++) {
        CNIT_ASSERT(llmap_find(map, i) == ((i % 6 == 0 && i % 15 != 0) ? i + 1 : -1));
    }

    llmap_free(map);
    return 0;
}

int main() {
    cnit_add_test(test_hash, "Hashing functions");
    cnit_add_test(test_map_insert, "Map insert/find operations");
    cnit_add_test(test_map_erase, "Map insert/erase operations");
    return cnit_run_tests();
}
