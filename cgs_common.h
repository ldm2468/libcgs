#ifndef CGS_COMMON_H
#define CGS_COMMON_H

#define CGS_CAT(a, b) a##b
#define CGS_CAT2(a, b) CGS_CAT(a, b)
#define CGS_FUNCTION(name) CGS_CAT2(cgs_name, _##name)

#endif
