#define cgs_type double
#define cgs_name dlist
#include "cgs_list.h"
#define cgs_dlist 1

#define cgs_type int
#define cgs_name ilist
#include "cgs_list.h"
#define cgs_ilist 1

#define cgs_type char *
#define cgs_name slist
#include "cgs_list.h"
#define cgs_slist 1

#include "cnit/cnit_main.h"
#define TEST_COUNT 1000

int test_sanity() {
    slist *s = slist_new();
    slist_free(s);
    return 0;
}

int test_push_pop() {
    dlist *list = dlist_new();

    /* Push at the front, pop at the back (FIFO order) */
    for (int i = 0; i < TEST_COUNT; i++) {
        dlist_push_front(list, i);
    }

    CNIT_ASSERT(list->size == TEST_COUNT);

    for (int i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(dlist_pop_back(list) == i);
        CNIT_ASSERT(list->size == TEST_COUNT - 1 - i);
    }

    /* Push at the back, pop at the front (Also FIFO) */
    for (int i = 0; i < TEST_COUNT; i++) {
        dlist_push_back(list, i);
    }

    CNIT_ASSERT(list->size == TEST_COUNT);

    for (int i = 0; i < TEST_COUNT; i++) {
        CNIT_ASSERT(dlist_pop_front(list) == i);
        CNIT_ASSERT(list->size == TEST_COUNT - 1 - i);
    }

    dlist_free(list);
    return 0;
}

int test_foreach() {
    ilist *list = ilist_new();
    ilist_push_back(list, 4);
    ilist_push_back(list, 1);
    ilist_push_back(list, 2);
    ilist_push_back(list, 3);
    ilist_push_back(list, 4);
    ilist_push_back(list, 1);

    CNIT_ASSERT(list->size == 6);

    {
        int target[] = {4, 1, 2, 3, 4, 1};
        int j = 0;
        cgs_list_foreach(ilist, list, n, i) {
            CNIT_ASSERT(i == target[j++]);
            if (i <= 2) {
                ilist_insert_after(list, n, 0);
            } else {
                ilist_insert_before(list, n, 5);
            }
        }
    }

    CNIT_ASSERT(list->size == 12);

    {
        int target[] = {5, 4, 1, 0, 2, 0, 5, 3, 5, 4, 1, 0};
        int j = 0;
        cgs_list_foreach(ilist, list, _, i) {
            CNIT_ASSERT(i == target[j++]);
        }
    }

    ilist_free(list);
    return 0;
}

int main() {
    cnit_add_test(test_sanity, "List sanity test");
    cnit_add_test(test_push_pop, "List push/pop");
    cnit_add_test(test_foreach, "List foreach");
    return cnit_run_tests();
}