#include "common.h"

s32 ovl_11_func_80104EC8(s32 arg0);

s32 ovl_11_func_800F1AA8(u8 arg0) {
    s32 result;

    if (arg0 != 0xFF) {
        result = ovl_11_func_80104EC8(arg0);
    } else {
        result = 1;
    }
    return result;
}
