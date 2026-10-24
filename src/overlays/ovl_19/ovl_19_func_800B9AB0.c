#include "common.h"
#include "game_types.h"

/* The overlay's symbol for Rand, whose matched definition returns u32. */
u32 Rand(s32 arg0);
s32 func_8001FABC(s16 arg0);
s32 ovl_19_func_800BA564(s16 *arg0, s16 *arg1);
s32 ovl_19_func_800BA5B4(s16 *arg0, s16 *arg1);
s32 ovl_19_func_800BA65C(Ovl19Func800BAC40Arg *arg0);
void ovl_19_func_800BAC40(Ovl19Func800BAC40Arg *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_19_func_800BA0A0(Recon_ovl_19_func_800BA0A0_A0View *arg0);

void ovl_19_func_800B9AB0(s16 *arg0) {
    s16 *other;
    s16 *base;
    s16 *state;
    s16 sum;
    s16 limit;
    s16 direction;
    s32 band;
    s32 roll;
    s16 flag;
    s32 i;

    other = D_800BF4C0 + 8;
    base = other;
    if (arg0 == other) {
        other = arg0 + 0x24;
    }
    state = base - 8;
    if ((arg0[3] == (arg0[3] / state[0xCA]) * state[0xCA]) || (arg0[3] == state[0xBC])) {
        roll = (s32)Rand(100);
        flag = arg0[4] * 100 / 255 < roll;
        band = ovl_19_func_800BA5B4(arg0, other);
        sum = 0;
        limit = (s16)Rand(100);
        for (i = 0; i < 4; i++) {
            sum = (s16)(sum + D_800BCF58[i + band * 4 + flag * 16]);
            if (sum >= limit) {
                direction = (s16)(arg0[2] + i);
                direction %= 4;
                ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)arg0, arg0[1], direction, arg0[3]);
                break;
            }
        }
    }
    if (arg0[3] == (arg0[3] / 2) * 2) {
        if (ovl_19_func_800BA5B4(arg0, other) == 0 && ovl_19_func_800BA564(arg0, other) < 201) {
            if (arg0[4] >= 240 && Rand(128) >= 111U) {
                D_800BF4C0[0x101] = func_8001FABC(0x3B);
                ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)arg0, 4, arg0[2], 30);
            } else {
                ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)arg0, 2, arg0[2], arg0[3]);
            }
        } else if (ovl_19_func_800BA65C((Ovl19Func800BAC40Arg *)arg0) == 1) {
            if ((s32)Rand(128) < arg0[4]) {
                direction = (s16)(arg0[2] + ovl_19_func_800BA5B4(arg0, other));
                ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)arg0, 2, (s16)(direction % 4), arg0[3]);
            } else if (Rand(128) < 111U) {
                ovl_19_func_800BAC40((Ovl19Func800BAC40Arg *)arg0, 2, arg0[2], arg0[3]);
            }
        }
        if (arg0[1] == 3) {
            ovl_19_func_800BA0A0((Recon_ovl_19_func_800BA0A0_A0View *)arg0);
        }
    }
}
