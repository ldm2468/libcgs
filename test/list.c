#define cgs_list_type double
#define cgs_list_name dlist
#include "cgs_list.h"
#define cgs_dlist 1

#define cgs_list_type int
#define cgs_list_name ilist
#include "cgs_list.h"
#define cgs_ilist 1

#define cgs_list_type long long
#define cgs_list_name llist
#include "cgs_list.h"
#define cgs_llist 1

#define cgs_list_type char *
#define cgs_list_name slist
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

int test_splice() {
    llist *l0 = llist_new(), *l1 = llist_new(), *l2 = llist_new();
    for (int i = 0; i < 100; i++) {
        llist_push_back(l0, i);
        llist_push_back(l1, 100 + i);
        llist_push_back(l2, 200 + i);
    }

    llist_node *l1_center = llist_find(l1, 150);

    llist_splice_after(l1, l1_center, l0);
    CNIT_ASSERT(l0->size == 0);
    CNIT_ASSERT(l1->size == 200);

    llist_splice_before(l2, llist_front_node(l2), l1);
    CNIT_ASSERT(l1->size == 0);
    CNIT_ASSERT(l2->size == 300);

    int i = 0;
    cgs_list_foreach(llist, l2, n, e) {
        if (i <= 50) {
            CNIT_ASSERT(e == i + 100);
        } else if (i <= 150) {
            CNIT_ASSERT(e == i - 51);
        } else {
            CNIT_ASSERT(e == i);
        }
        i++;
    }

    llist_free(l0);
    llist_free(l1);
    llist_free(l2);

    return 0;
}

int main() {
    cnit_add_test(test_sanity, "List sanity test");
    cnit_add_test(test_push_pop, "List push/pop");
    cnit_add_test(test_foreach, "List foreach");
    cnit_add_test(test_splice, "List splice");
    return cnit_run_tests();
}