#include "common.h"

/* Rand at 0x80012A34; use the overlay link's imported symbol name. */
u32 func_80012A34(s32 arg0);
s32 func_8001AF44(u32 arg0);
s32 ovl_11_func_800EFA1C(s16 arg0);
s32 ovl_11_func_800EDF3C();

s32 ovl_11_func_800ED7E0(s16 arg0, s16 arg1) {
    struct Ovl11CandidateList choices = D_800B9DB0;
    s16 count = 0;
    s16 i;
    u16 highest;
    u16 value;
    char *base;
    char *base2;
    char *p;
    char *p2;

    if (arg0 == 0) {
        for (i = 0; i < 4; i++) {
            if (func_8001AF44((u16)(i + 0x21)) == 1) {
                choices.values[i] = -1;
                count++;
            }
        }
        if (ovl_11_func_800EFA1C(0) == 1) {
            choices.values[4] = -1;
            count++;
        }
    }
    if (choices.values[4] != -1) {
        if (ovl_11_func_800EDF3C(choices.values[4], 0, 0, 0) == 0) {
            choices.values[4] = -1;
            count++;
        }
    }
    highest = 0;
    if (count == 5) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 5; i++) {
        if (choices.values[i] != -1) {
            base = (char *)&D_8006C838;
            p = choices.values[i] * 0x1D4 + base;
            value = *(u16 *)(p + 0x8000 + 0x19EC);
            if (highest < value) {
                highest = value;
                count = 1;
            } else if (highest == value) {
                count++;
            }
        }
    }
    count = func_80012A34((u16)count);
    for (i = 0; i < 5; i++) {
        if (choices.values[i] != -1) {
            base2 = (char *)&D_8006C838;
            p2 = choices.values[i] * 0x1D4 + base2;
            if (highest == *(u16 *)(p2 + 0x8000 + 0x19EC)) {
                if (count == 0) {
                    break;
                }
                count--;
            }
        }
    }
    if (arg1 != -1) {
        D_80129560[arg1] = choices.values[i];
    }
    return 1;
}
