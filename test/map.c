#include <stdint.h>

#define cgs_key int
#define cgs_value int
static inline uint32_t iimap_hash(int key) {
    return key;
}
#define cgs_name iimap
#include "cgs_map.h"
#define cgs_iimap 1

#define cgs_key char *
#define cgs_value int
#define cgs_map_default_hash_str
#define cgs_name simap
#include "cgs_map.h"
#define cgs_simap 1

#define cgs_key long
#define cgs_value long
#define cgs_map_default_hash
#define cgs_name llmap
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

int main() {
    cnit_add_test(test_hash, "Test hashing functions");
    return cnit_run_tests();
}
