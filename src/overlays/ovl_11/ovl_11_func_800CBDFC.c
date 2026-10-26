#include "common.h"

extern s32 gPrimaryObjectFlags;

s32 ovl_11_func_800CBDFC(void) {
    s32 t = gPrimaryObjectFlags & 0x20000000;
    return t == 0;
}
