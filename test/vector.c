#define cgs_type double
#define cgs_name double_vec
#include "cgs_vector.h"
#define cgs_double_vec 1
#define cgs_type int
#define cgs_name int_vec
#include "cgs_vector.h"
#define cgs_int_vec 1
#define cgs_type char *
#define cgs_name str_vec
#include "cgs_vector.h"
#define cgs_str_vec 1
#define cgs_type double
#define cgs_name double_vec
#include "cgs_vector.h"
#define cgs_double_vec 1

#include <stdio.h>

int main() {
    double_vec *v = double_vec_new();
    double_vec_push_back(v, 20.);
    double_vec_push_back(v, 10.);
    printf("%lf, %lf\n", double_vec_at(v, 0), double_vec_at(v, 1));
    double_vec_free(v);

    int_vec *i = int_vec_new();
    int_vec_push_back(i, 30);
    int_vec_push_back(i, 15);
    printf("%d, %d\n", int_vec_at(i, 0), int_vec_at(i, 1));
    int_vec_free(i);

    str_vec *s = str_vec_new();
    str_vec_push_back(s, "test");
    str_vec_push_back(s, "hi");
    printf("%s, %s\n", str_vec_at(s, 0), str_vec_at(s, 1));
    str_vec_free(s);
}
