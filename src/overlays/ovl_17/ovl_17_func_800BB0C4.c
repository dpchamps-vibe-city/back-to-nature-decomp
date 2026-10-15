#include "common.h"

void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_8001AAF4(s16 arg0, s16 arg1, s32 arg2, s16 arg3, s16 arg4);
void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_17_func_800BB0C4(s16 arg0) {
    s32 display;

    display = D_8005E3C0->field_D8 + 8;
    if (arg0 == -1) {
        D_800BB724 = 0;
    }
    if (arg0 == 0 || arg0 == 2) {
        D_800BB724++;
    }
    if ((u16)arg0 < 2) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0xC), 1, 0xA, 0x14, 0x6B, 0x11);
        func_8001AAF4(D_800BB724 / 1800 - (D_800BB724 / 108000) * 60, 2, display, 0xC, 0x16);
        func_80017B3C(display, (s32)D_800BB6E8, 0x26, 0x16);
        func_8001AAF4(D_800BB724 / 300 - (D_800BB724 / 1800) * 6, 1, display, 0x33, 0x16);
        func_8001AAF4(D_800BB724 / 30 - (D_800BB724 / 300) * 10, 1, display, 0x40, 0x16);
        func_80017B3C(display, (s32)D_800BB6E8, 0x4D, 0x16);
        func_8001AAF4(D_800BB724 / 3 - (D_800BB724 / 30) * 10, 1, display, 0x5A, 0x16);
        func_8001AAF4((D_800BB724 * 100) / 30 - (D_800BB724 / 3) * 10, 1, display, 0x67, 0x16);
    }
}
