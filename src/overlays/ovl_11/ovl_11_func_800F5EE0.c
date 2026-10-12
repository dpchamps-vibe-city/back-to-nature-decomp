#include "common.h"

s32 ovl_11_func_800F5EE0(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 result;

    result = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x47:
        *arg2 = 0x17;
        break;
    case 0x51:
        break;
    case 0x52:
        *arg2 = 1;
        break;
    case 0x53:
        *arg2 = 2;
        break;
    case 0x59:
    case 0x139:
        *arg2 = 0x1D;
        break;
    case 0x84:
        *arg2 = 7;
        break;
    case 0x6D:
        *arg2 = 8;
        break;
    case 0x6E:
        *arg2 = 9;
        break;
    case 0x177:
        *arg2 = 0xA;
        break;
    case 0xA5:
        *arg2 = 0xC;
        break;
    case 0xA6:
        *arg2 = 0xD;
        break;
    case 0xA7:
        *arg2 = 0xE;
        break;
    case 0x128:
        *arg2 = 0xF;
        break;
    case 0x129:
        *arg2 = 0x10;
        break;
    case 0x12A:
        *arg2 = 0x11;
        break;
    case 0x54:
        *arg2 = 0xB;
        break;
    case 0x64:
        *arg2 = 0x18;
        break;
    case 0x9A:
        *arg2 = 0x19;
        break;
    case 0x99:
        *arg2 = 0x1A;
        break;
    case 0x21:
        *arg2 = 0x1B;
        break;
    case 0x6F:
        *arg2 = 0x1C;
        break;
    case 0x65:
        *arg2 = 0x1E;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}
