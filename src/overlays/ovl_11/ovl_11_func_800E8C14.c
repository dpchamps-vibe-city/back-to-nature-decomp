#include "common.h"

s32 ovl_11_func_800D60D4(s16 *arg0);
s32 ovl_11_func_800D60F0(s16 arg0, s16 arg1);

s32 ovl_11_func_800E8C14(s16 arg0, s16 arg1, s32 arg2) {
    s32 value;
    s32 check_result;
    s32 result;
    s32 index;

    if (arg2 != 0) {
        index = D_80129560[arg0];
    } else {
        index = arg0;
    }
    value = ((struct Ovl11E8C14State *)D_8006C838)->value;
    if (value == 0) {
        return 0;
    }
    check_result = ovl_11_func_800D60D4(&((struct Ovl11E8C14State *)D_8006C838)->value);
    if (check_result != 0) {
        value = check_result;
    }
    result = ovl_11_func_800D60F0(value, index);
    if (result < 7) {
        if (result >= 0) {
            D_80129560[arg1] = result + 1;
            return 1;
        }
    }
    D_80129560[arg1] = 0;
    return 1;
}
