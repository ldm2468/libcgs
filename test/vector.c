#define cgs_type double
#define cgs_name dvec
#include "cgs_vector.h"
#define cgs_dvec 1

#define cgs_type int
#define cgs_name ivec
#include "cgs_vector.h"
#define cgs_ivec 1

#define cgs_type char *
#define cgs_name svec
#include "cgs_vector.h"
#define cgs_svec 1

#include "cnit/cnit_main.h"
#define TEST_COUNT 1000

int test_sanity() {
    svec *v = svec_new();
    CNIT_ASSERT(v->size == 0);
    CNIT_ASSERT(v->capacity > 0);
    svec_free(v);
    return 0;
}

int test_stack_ops() {
    ivec *v = ivec_new();
    int i;
    /* push: 0, 1, ..., TEST_COUNT - 1 */
    for (i = 0; i < TEST_COUNT; i++) {
        ivec_push_back(v, i);
        CNIT_ASSERT(v->size == i + 1);
        CNIT_ASSERT(v->capacity >= v->size);
    }
    for (i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(ivec_at(v, i) == i);
    }
    /* pop: TEST_COUNT - 1, ..., 1, 0 (LIFO order) */
    for (i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(ivec_pop_back(v) == TEST_COUNT - 1 - i);
        CNIT_ASSERT(v->size == TEST_COUNT - 1 - i);
    }
    ivec_free(v);
    return 0;
}

int test_insert_erase() {
    dvec *v = dvec_new();
    int i;
    /* Insert at the beginning: TEST_COUNT - 1, ..., 1, 0 */
    for (i = 0; i < TEST_COUNT; i++) {
        dvec_insert(v, 0, i);
        CNIT_ASSERT(v->size == i + 1);
        CNIT_ASSERT(v->capacity >= v->size);
    }
    for (i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(dvec_at(v, i) == TEST_COUNT - 1 - i);
    }
    /* Erase from the beginning: TEST_COUNT - 1, ..., 1, 0 */
    for (i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(dvec_erase(v, 0) == TEST_COUNT - 1 - i);
        CNIT_ASSERT(v->size == TEST_COUNT - 1 - i);
    }
    /* Insert at the end: TEST_COUNT, TEST_COUNT + 1, ..., 2 * TEST_COUNT - 1 */
    for (i = 0; i < TEST_COUNT; i++) {
        dvec_insert(v, i, TEST_COUNT + i);
    }
    /* Insert at the middle: 0, 1, ..., 2 * TEST_COUNT - 1 */
    for (i = 0; i < TEST_COUNT; i++) {
        dvec_insert(v, i, i);
    }
    for (i = 0; i < 2 * TEST_COUNT; i++) {
        CNIT_ASSERT(dvec_at(v, i) == i);
    }
    /* Remove at the middle: TEST_COUNT, TEST_COUNT + 1, ..., 2 * TEST_COUNT - 1 */
    for (i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(dvec_erase(v, TEST_COUNT) == TEST_COUNT + i);
    }
    dvec_free(v);
    return 0;
}

int main() {
    cnit_add_test(test_sanity, "Vector sanity test");
    cnit_add_test(test_stack_ops, "Vector stack operations (push/pop)");
    cnit_add_test(test_insert_erase, "Vector insert/erase operations");
    return cnit_run_tests();
}
