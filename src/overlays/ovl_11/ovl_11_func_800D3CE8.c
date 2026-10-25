#include "common.h"
#include "random.h"

s32 ovl_11_func_800D3CE8(void *arg0) {
    s32 var_a0 = 2;
    s32 var_v1;

    if (*(u32 *)((char *)arg0 + 0x34) & 0x2000) {
        var_a0 = 7;
    }
    if (Rand(var_a0) == 0) {
        var_v1 = 0xD;
    } else {
        var_v1 = 1;
    }
    return var_v1;
}
