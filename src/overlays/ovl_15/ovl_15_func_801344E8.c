#include "common.h"

s32 ovl_15_func_801344E8(u8 *arg0, s16 arg1) {
    s32 var_t0;
    s16 var_t6;
    s32 var_v0;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;
    u8 *var_a0;

    var_t0 = 0;
    var_t6 = 0;
    switch (D_8013759E) {
    case 0: {
        s32 temp;
        s16 m1;
        s32 m2;
        s16 m3;
        s32 m4;
        var_a2 = 0;
        m1 = 0x10A;
        m2 = 0x02020000;
        m3 = 0x109;
        m4 = 0x40000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((*(u16 *) (var_a0 + 0) == m1 && !(*(s32 *) (var_a0 + 0x34) & m2)) || *(u16 *) (var_a0 + 0) == m3) {
                if (!(*(s32 *) (var_a0 + 0x34) & m4)) {
                    temp = var_a3;
                    var_a3 += 0x10000;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB4;
        } while (var_a2 < 0xA);
        break;
    }
    case 1: {
        s32 temp;
        var_a2 = 0;
        var_v1 = 0x10000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((u32) ((*(u16 *) (var_a0 + 0x708)) - 0x160) < 3U) {
                if ((*(u16 *) (var_a0 + 0x7BA)) == 0) {
                    temp = var_v1;
                    var_v1 += var_a3;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB8;
        } while (var_a2 < 0x14);
        break;
    }
    case 2: {
        s32 temp;
        var_a2 = 0;
        var_v1 = 0x10000;
        var_a3 = 0x10000;
        var_a0 = arg0;
        do {
            if ((u32) ((*(u16 *) (var_a0 + 0x708)) - 0x164) < 2U) {
                if ((*(u16 *) (var_a0 + 0x7BA)) == 0) {
                    temp = var_v1;
                    var_v1 += var_a3;
                    var_t0 = temp >> 0x10;
                }
                if (var_t0 == arg1) {
                    var_t6 = var_a2;
                    break;
                }
            }
            var_a2 += 1;
            var_a0 += 0xB8;
        } while (var_a2 < 0x14);
        break;
    }
    default:
        break;
    }
    var_v0 = var_t0;
    if (arg1 == -1) {
        return var_v0;
    }
    return var_t6;
}
