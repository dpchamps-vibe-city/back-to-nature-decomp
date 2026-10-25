#include "common.h"

s32 ovl_11_func_800F5D04(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_a3;

    var_a3 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x3A:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 0;
        break;
    case 0x3B:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 1;
        break;
    case 0x3C:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 2;
        break;
    case 0x3D:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 3;
        break;
    case 0x3E:
        if (gCalendarSeason == 3) {
            *arg1 = 2;
        }
        *arg2 = 4;
        break;
    case 0x40:
        if (gCalendarSeason == 2) {
            *arg1 = (s32) gCalendarSeason;
        }
        *arg2 = 5;
        break;
    case 0x51:
        *arg2 = 6;
        break;
    case 0x52:
        *arg2 = 7;
        break;
    case 0x53:
        *arg2 = 8;
        break;
    case 0x54:
        *arg2 = 0x13;
        break;
    case 0x55:
        *arg2 = 9;
        break;
    case 0x56:
        *arg2 = 0xA;
        break;
    case 0x58:
        *arg2 = 0xB;
        break;
    case 0x59:
        *arg2 = 0xC;
        break;
    case 0x5A:
        *arg2 = 0xD;
        break;
    case 0x5B:
        *arg2 = 0xE;
        break;
    case 0x5C:
        *arg2 = 0xF;
        break;
    case 0x5D:
        *arg2 = 0x10;
        break;
    case 0x5E:
        *arg2 = 0x11;
        break;
    case 0x5F:
        *arg2 = 0x12;
        break;
    case 0x65:
        *arg2 = 0x15;
        break;
    case 0x64:
        *arg2 = 0x20;
        break;
    default:
        var_a3 = 0;
        break;
    }
    return var_a3;
}
