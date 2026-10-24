#include "common.h"

void ovl_23_func_800B9310(void);
void func_80017A08(s32 arg0, s32 arg1);
s32 func_8002261C(s32 arg0, s32 arg1);
/* Executable import GetVal8005E5B4. */
s32 GetVal8005E5B4(void);
void func_80022738(void);
/* Executable imports SetVal8005E2BC and SetVal8005E334. */
void SetVal8005E2BC(s32 arg0);
void SetVal8005E334(s32 arg0);

void ovl_23_func_800B8644(void) {
    ovl_23_func_800B9310();
    func_80017A08(0, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[0]]);
    func_80017A08(1, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[1]]);
    func_80017A08(2, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[3]]);
    func_80017A08(3, (s32)D_800BB860 + D_800BBA14[Ovl23_D_800BF87C_header.unk16[5]]);
    func_8002261C(4, 0x2D);
    if (GetVal8005E5B4() == 2) {
        func_80022738();
        SetVal8005E2BC(0);
        SetVal8005E334(0);
        Ovl23_D_800BF87C_header.unk0 = 5;
        Ovl23_D_800BF87C_header.unk416 = 0;
    }
}
