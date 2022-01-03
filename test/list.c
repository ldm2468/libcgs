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

int main() {
    cnit_add_test(test_sanity, "List sanity test");
    return cnit_run_tests();
}