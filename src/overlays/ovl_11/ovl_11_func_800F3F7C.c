#include "common.h"

extern s32 gProgressStateFlags;

s32 ovl_11_func_800F3F7C(void) {
    if ((gProgressStateFlags & 0x04000400) == 0x400) {
        gProgressStateFlags |= 0x04000000;
        return 1;
    }
    return 0;
}
