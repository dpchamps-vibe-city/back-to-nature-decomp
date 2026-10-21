#include "common.h"


s32 ovl_11_func_800D9E10(s32 arg0, u8 arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 result;

    result = 1;
    *arg2 = 0;
    *arg3 = 0;
    *arg4 = 0;
    switch (arg0) {
    case 0x3F:
        *arg3 = 5;
        break;
    case 0x175:
        *arg2 = 1;
        *arg3 = 5;
        break;
    case 0x3A:
        *arg3 = 0;
        break;
    case 0x3E:
        *arg3 = 4;
        break;
    case 0x16D:
        *arg3 = 1;
        *arg4 = -10;
        break;
    case 0x170:
        *arg3 = 2;
        *arg4 = -10;
        break;
    case 0x173:
        *arg3 = 3;
        *arg4 = -15;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
