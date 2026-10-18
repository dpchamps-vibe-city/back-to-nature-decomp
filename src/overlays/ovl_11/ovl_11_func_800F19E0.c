#include "common.h"

s32 ovl_11_func_800F19C8(s32 arg0);
/* This caller explicitly supplies an unread fourth argument in $a3. */
s32 ovl_11_func_800E9914(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800F19E0(u8 arg0, u8 arg1) {
    s32 result;

    if (arg0 == 1) {
        return 1;
    }
    result = 0;
    if (arg0 == D_8012961C) {
        result = 1;
        switch (arg0) {
        case 2:
            break;
        case 3:
            if (ovl_11_func_800F19C8(arg1) == 0) {
                result = 0;
            }
            break;
        case 4:
            if (((struct Ovl11E9914State *)D_8006C838)->pending != 0x186) {
                result = ovl_11_func_800E9914(0, arg1, -1, 0);
                ((struct Ovl11E9914State *)D_8006C838)->pending = 0;
            } else {
                result = 0;
            }
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            break;
        }
    }
    return result;
}
