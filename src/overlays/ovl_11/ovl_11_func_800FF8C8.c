#include "common.h"

extern s32 D_801273B0;

s32 func_8002261C(s32 arg0, s32 arg1);
s32 GetVal8005E5B4(void);
void func_80022738(void);
void SetVal8005E334(s32 arg0);
void SetVal8005E2BC(s32 arg0);

void ovl_11_func_800FF8C8(s16 arg0, s16 arg1) {
    s32 var_a1;

    if (D_801273B0 == 0x21) {
        if (arg1 == 0) {
            var_a1 = arg0 + 0x37B;
        } else {
            var_a1 = arg1 + 0x37E;
        }
        func_8002261C(3, var_a1);
        if (GetVal8005E5B4() == 2) {
            func_80022738();
            D_801273B0 = 0x20;
        }
    }
    if (D_801273B0 == 0x20) {
        SetVal8005E334(0);
        SetVal8005E2BC(0);
        func_8002261C(3, 0x3B3);
    }
}
