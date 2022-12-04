# libcgs
**C** **G**eneric Data **S**tructures

A header-only C library that provides basic STL-like generic data structures.
Currently, vectors, lists, and (unordered) maps are supported.

## Overview
This library allows users to generate data structures for arbitrary element
types. For example, the following code generates an integer vector type `ivec`
and associated functions with the `ivec` prefix.

```C
#define cgs_vec_type int
#define cgs_vec_name ivec
#include "cgs_vector.h"
#define cgs_ivec 1

int main() {
    ivec *vector = ivec_new();

    ivec_push_back(vector, 10);
    printf("%d\n", ivec_at(vector, 0));

    ivec_free(vector);
    return 0;
}
```

Before including the header, some macros must be defined to specify the
name and the type of the generated container. The line `#define cgs_ivec 1`
acts as an include guard to prevent multiple definition errors.

If multiple versions of the same data structure is required, (e.g. an integer
vector and a double vector), the header can simply be included multiple times,
as long as unique names are used for the container names.

The following code generates two containers: a double list `dlist` and an
integer list `ilist`.
```C
#define cgs_list_type double
#define cgs_list_name dlist
#include "cgs_list.h"
#define cgs_dlist 1

#define cgs_list_type int
#define cgs_list_name ilist
#include "cgs_list.h"
#define cgs_ilist 1
```

The list data structure also supports convenient iteration with a
for-each macro.

```C
#define cgs_list_type int
#define cgs_list_name ilist
#include "cgs_list.h"
#define cgs_ilist 1

int main() {
    ilist *l = ilist_new();

    for (int i = 0; i < 10; i++) {
        ilist_push_back(l, i);
    }

    cgs_list_foreach(ilist, l, _, e) {
        printf("%d\n", e);
    }

    ilist_free(l);
    return 0;
}
```

## Usage
This library is header-only, so simply including the headers in your project
is sufficient.

## Documentation
A `Doxyfile` is included to help generate doxygen documentation. The generated
doc files can be a bit rough around the edges, but are usable as a basic
reference. The unit tests at `test/*.c` are also useful examples. 
