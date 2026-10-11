#include "common.h"

s32 ovl_11_func_800CBDFC(void);
void ovl_11_func_800D2764(s32 arg0);

void ovl_11_func_800D2710(void) {
    if ((D_80128D68 != -1) && (ovl_11_func_800CBDFC() != 0)) {
        ovl_11_func_800D2764((s32) D_80128D68);
        D_80128D68 = -1;
    }
}
