#include "common.h"

void ovl_23_func_800B9310(void);
void func_80022738(void);
/* Executable imports: SetVal8005E2BC and SetVal8005E334. */
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
/* The matched wrapper forwards func_800212A8's s32 voice index. */
s32 func_8001FABC(s16 arg0);

void ovl_23_func_800B8754(void) {
    ovl_23_func_800B9310();
    if (Ovl23_D_800BF87C_header.unk416 == 0) {
        func_80022738();
        SetVal8005E2BC(0);
        SetVal8005E334(0);
        func_8002261C(4, 0x2E);
    }
    Ovl23_D_800BF87C_header.unk416++;
    if (Ovl23_D_800BF87C_header.unk416 >= 60) {
        Ovl23_D_800BF87C_header.unk414 = func_8001FABC(0x38);
        func_80022738();
        Ovl23_D_800BF87C_header.unk0 = 6;
        Ovl23_D_800BF87C_header.unk416 = 0;
    }
}
