#include "common.h"

s32 ovl_11_func_800EE000(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 temp_a2;
    s16 temp_t0;
    s16 temp_t1;
    s16 var_v0;
    s16 var_a0;
    s32 var_a1;

    temp_t0 = *(s16 *)&D_80129560[arg0];
    temp_t1 = *(s16 *)&D_80129560[arg1];
    temp_a2 = *(s16 *)&D_80129560[arg2];
    switch (temp_t0) {
    case 19:
        var_a0 = 0;
        var_a1 = 0x14;
        break;
    case 20:
        var_a0 = 1;
        var_a1 = 0x14;
        break;
    case 16:
        var_a0 = 2;
        var_a1 = 9;
        break;
    case 5:
        var_a0 = 3;
        var_a1 = 7;
        break;
    case 21:
    case 22:
        var_a1 = 0x16;
        var_a0 = 4;
        break;
    case 34:
        var_a0 = 5;
        var_a1 = 9;
        break;
    case 9:
        var_a0 = 6;
        var_a1 = 9;
        break;
    case 11:
        var_a1 = 0x16;
        var_a0 = 7;
        break;
    case 6:
        var_a0 = 8;
        var_a1 = 7;
        break;
    case 7:
        var_a0 = 9;
        var_a1 = 7;
        break;
    case 25:
        var_a1 = 0x14;
        var_a0 = 0xA;
        break;
    default:
        var_a0 = 0xB;
        var_a1 = -1;
        break;
    }
    if (temp_a2 == -1) {
        if (var_a0 < 5) {
            if (temp_t0 == 0x16) {
                var_v0 = var_a0 + 0x70E;
            } else {
                var_v0 = var_a0 + 0x70D;
            }
        } else {
            var_v0 = var_a0 + 0x70E;
        }
    } else if (temp_t1 == temp_a2) {
        var_a0 *= 2;
        switch (temp_t0) {
        case 5:
        case 11:
        case 16:
        case 25:
            if (temp_a2 == var_a1) {
                var_v0 = var_a0 + 0x548;
            } else {
                var_v0 = var_a0 + 0x549;
            }
            break;
        default:
            if (temp_a2 == var_a1) {
                var_v0 = var_a0 + 0x549;
            } else {
                var_v0 = var_a0 + 0x548;
            }
            break;
        }
    } else {
        var_a0 *= 3;
        switch (temp_t0) {
        case 5:
        case 11:
        case 16:
        case 25:
            if (temp_a2 == var_a1) {
                var_v0 = var_a0 + 0x526;
            } else if (temp_t1 == var_a1) {
                var_v0 = var_a0 + 0x527;
            } else {
                var_v0 = var_a0 + 0x528;
            }
            break;
        default:
            if (temp_a2 == var_a1) {
                var_v0 = var_a0 + 0x528;
            } else if (temp_t1 == var_a1) {
                var_v0 = var_a0 + 0x527;
            } else {
                var_v0 = var_a0 + 0x526;
            }
            break;
        }
    }
    D_80129560[arg3] = var_v0;
    return 1;
}
