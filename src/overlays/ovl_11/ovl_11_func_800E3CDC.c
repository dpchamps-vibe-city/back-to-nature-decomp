#include "common.h"

s32 func_8001AF44(u32 arg0);

s32 ovl_11_func_800E351C(u8 *arg0, s16 arg1, s16 arg2);

void ovl_11_func_800E3CDC(u8 *arg0) {
    char *far_base;
    s16 var_s0;

    if ((func_8001AF44(2U) != 1) && !(D_8006C844 & 0x18000)) {
        far_base = (char *)&D_8007AFF0;
        if (*(s16 *)(far_base + 0x25476) != 0x2C) {
            var_s0 = ((*(u16 *)(*(s32 *)(far_base + 0x25388) + 8) & 0x8000) == 0) << 8;
            if (*(s16 *)(far_base + 0x25476) == 0x1B) {
                var_s0 = 0;
            }
            ovl_11_func_800E351C(arg0, var_s0, 0);
            D_80129418 = var_s0;
        }
    }
}
