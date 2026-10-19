#include "common.h"

s32 ovl_11_func_801075C0(s32 arg0, s32 arg1);
s32 ovl_11_func_80107604(s32 arg0, s32 arg1);
s32 ovl_11_func_8010775C(u32 arg0);
s32 ovl_11_func_80107888(s32 arg0, s32 arg1);

s32 ovl_11_func_80107528(s32 arg0, u16 arg1) {
    s32 result;

    switch (arg0) {
    case 0:
        result = ovl_11_func_801075C0(arg1 >> 8, arg1 & 0xFF);
        break;
    case 1:
        result = ovl_11_func_80107604(arg1 >> 8, arg1 & 0xFF);
        break;
    case 2:
        result = ovl_11_func_8010775C(arg1);
        break;
    case 3:
        result = ovl_11_func_80107888(arg1, arg1);
        break;
    case 0xFF:
    default:
        result = 0;
        break;
    }
    return result;
}
