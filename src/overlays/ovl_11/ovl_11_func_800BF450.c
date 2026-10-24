#include "common.h"

/* Pointer-returning zero-argument interface also used by the matched
 * BF3D0/BF3F4 siblings; the getter itself has no published definition. */
void *func_8001EF98(void);

static __inline__ unsigned long long flag_result(u32 high) {
    union {
        struct {
            u32 low;
            u32 high;
        } words;
        unsigned long long wide;
    } result = {{0, high}};

    return result.wide;
}

static __inline__ unsigned long long status_result(u32 low) {
    union {
        u32 low;
        unsigned long long wide;
    } result = {low};

    return result.wide;
}

unsigned long long ovl_11_func_800BF450(void) {
    u32 *p;
    u32 v;
    u32 low;
    unsigned long long result;

    p = func_8001EF98();
    D_80128808 = p;
    if (p == 0) {
        return status_result(1);
    }
    v = *p;
    low = v & 0xFFFF;
    D_8012880C = low;
    if (v & 0x2000) {
        result = status_result(0);
    } else {
        result = flag_result(0x8000);
    }
    return result;
}
