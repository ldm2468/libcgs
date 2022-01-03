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

int test_foreach() {
    ilist *list = ilist_new();
    ilist_push_back(list, 1);
    ilist_push_back(list, 2);
    ilist_push_back(list, 3);
    ilist_push_back(list, 4);

    {
        int target[] = {1, 2, 3, 4};
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

    CNIT_ASSERT(list->size == 8);

    {
        int target[] = {1, 0, 2, 0, 5, 3, 5, 4};
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
    cnit_add_test(test_foreach, "List foreach");
    return cnit_run_tests();
}