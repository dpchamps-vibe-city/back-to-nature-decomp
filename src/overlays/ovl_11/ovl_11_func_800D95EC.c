#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800D9EFC(s32 arg0, s32 arg1, s32 arg2, s32 *arg3, s32 *arg4);

s32 ovl_11_func_800D95EC(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
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
    case 0x40:
        *arg3 = 6;
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
    case 0x64:
        *arg3 = 15;
        break;
    case 0x22:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 7, arg2, arg3);
        break;
    case 0x23:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 8, arg2, arg3);
        break;
    case 0x24:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 9, arg2, arg3);
        break;
    case 0x25:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 10, arg2, arg3);
        break;
    case 0x26:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 11, arg2, arg3);
        break;
    case 0x31:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 12, arg2, arg3);
        break;
    case 0x34:
        result = ovl_11_func_800D9EFC(arg1, arg1 + 2, 13, arg2, arg3);
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
