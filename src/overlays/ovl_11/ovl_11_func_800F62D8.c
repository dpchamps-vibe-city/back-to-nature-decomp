#include "common.h"

s32 ovl_11_func_800F62D8(s32 arg0, s32 *arg1, s32 *arg2) {
    s32 var_a0;
    s32 var_a3;
    s32 var_t0;

    var_t0 = 1;
    *arg1 = 0;
    *arg2 = 0;
    switch (arg0) {
    case 0x64:
        break;
    case 0x65:
        *arg2 = 1;
        break;
    case 0x188:
        *arg2 = 0x14;
        break;
    case 0x71:
        var_a0 = 0;
        var_a3 = 0x13;
        switch ((u16)D_80070D0E) {
        case 0x51:
            var_a0 = 0;
            break;
        case 0x55:
            var_a0 = 1;
            break;
        case 0x52:
            var_a0 = 2;
            break;
        case 0x53:
            var_a0 = 4;
            break;
        case 0x54:
            var_a0 = 5;
            break;
        default:
            var_a3 = 3;
            break;
        }
        *arg1 = var_a0;
        *arg2 = var_a3;
        break;
    case 0x72:
        *arg2 = 4;
        break;
    case 0x73:
        *arg2 = 5;
        break;
    case 0x74:
        *arg2 = 6;
        break;
    case 0x75:
        *arg2 = 7;
        break;
    case 0x76:
        *arg2 = 8;
        break;
    case 0x77:
        *arg2 = 9;
        break;
    case 0x78:
        *arg2 = 0xA;
        break;
    case 0x79:
        *arg2 = 0xB;
        break;
    case 0x7D:
        *arg2 = 0xC;
        break;
    case 0x7E:
        *arg2 = 0xD;
        break;
    case 0x7F:
        *arg2 = 0xE;
        break;
    case 0x80:
        *arg2 = 0xF;
        break;
    case 0x81:
        *arg2 = 0x10;
        break;
    case 0x82:
        *arg2 = 0x11;
        break;
    default:
        var_t0 = 0;
        break;
    }
    return var_t0;
}
