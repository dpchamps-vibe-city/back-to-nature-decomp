#include "common.h"

s32 ovl_11_func_800E60A0(s16 arg0, s32 height, s32 arg2, s32 arg3);

s32 ovl_11_func_800E880C(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 result;
    s32 index;
    u8 *position;

    if (arg1 != 0) {
        index = gOvl11ValueSlots[arg0];
    } else {
        index = arg0;
    }
    if (index == 0x29) {
        index = -2;
    }
    result = 1;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    if (index != -1 && arg2 != 0) {
        if (gOvl11SelectedPositionRecord == index) {
            return 1;
        }
        if (index == -2) {
            position = (u8 *)D_8006C838 + 0x52C8;
        } else {
            if ((u32)(index - 1) >= 0x24U) {
                return 0;
            }
            position = (u8 *)gObjectPositionRecordCoordinates + index * 0x1D4;
        }
        result = ovl_11_func_800E60A0(arg3, *(s16 *)(position + 4),
                                    *(s32 *)position, *(s32 *)(position + 8));
    }
    if (result == 1) {
        if (arg2 != 2) {
            gOvl11SelectedPositionRecord = index;
            if (index == -2) {
                ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x400000;
            }
        }
    }
    return result;
}
