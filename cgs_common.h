#ifndef CGS_COMMON_H
#define CGS_COMMON_H

#define CGS_CAT_HELPER(a, b) a##b
#define CGS_CAT_HELPER2(a, b) CGS_CAT_HELPER(a##_, b)
#define CGS_CAT_HELPER3(a, b) CGS_CAT_HELPER2(cgs_internal_##a, b)
#define CGS_CAT(a, b) CGS_CAT_HELPER2(a, b)
#define CGS_CAT_INTERNAL(a, b) CGS_CAT_HELPER3(a, b)

#endif
