#include "common.h"

s32 func_8001AF44(u32 arg0);

void ovl_11_func_800DD904(void) {
    s32 temp_v0;

    if (D_80129194 == 0) {
        return;
    }
    if (D_80129198 == 1) {
        goto L1;
    }
    if (D_80129198 == 0) {
        goto L0;
    }
    if (D_80129198 == 2) {
        goto L2;
    }
    if (D_80129198 == 3) {
        goto L3;
    }
    return;
L0:
    temp_v0 = func_8001AF44(0x71U);
    if (temp_v0 == 1) {
        D_80129198 = temp_v0;
        return;
    }
    return;
L1:
    D_80129194->unk14 += 0x20;
    if (D_80129194->unk14 >= 0x280) {
        D_80129194->unk14 = 0x280;
        D_80129198 = 2;
        return;
    }
    return;
L2:
    if (func_8001AF44(0x71U) == 0) {
        D_80129198 = 3;
        return;
    }
    return;
L3:
    D_80129194->unk14 -= 0x20;
    if (D_80129194->unk14 <= 0) {
        D_80129194->unk14 = 0;
        D_80129198 = 0;
    }
}
