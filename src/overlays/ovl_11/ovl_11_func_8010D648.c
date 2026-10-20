#include "common.h"

s32 func_80012A34(s32 arg0);
s32 ovl_11_func_800F3BA0(u16 arg0);
s32 ovl_11_func_800F3A18(u16 arg0);
void ovl_11_func_800D0EA4(s32 arg0, s32 arg1);
s32 ovl_11_func_80110944(s16 area, u16 mode);

s32 ovl_11_func_8010D648(u8 *arg0, s16 area) {
    s32 random;
    s32 failed;
    s32 i;
    s32 count;
    s16 index;

    random = func_80012A34(3);
    count = (u16)(*(u16 *)(arg0 + 0xB2) / 50U) + random;
    failed = 0;
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 1);
        if (index == -1) {
            failed = 1;
            break;
        }
        if (area == 4) {
            if (ovl_11_func_800F3BA0(1)) {
                ovl_11_func_800D0EA4(index, 0);
            } else {
                failed = 1;
            }
        } else if (area == 5) {
            if (!ovl_11_func_800F3A18(1)) {
                failed = 1;
            } else {
                ovl_11_func_800D0EA4(index, 1);
            }
        }
    }
    return failed;
}
