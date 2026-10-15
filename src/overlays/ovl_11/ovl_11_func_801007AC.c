#include "common.h"

void ovl_11_func_80100B50(u8 arg0, u8 arg1);
s32 func_8002261C(s32 arg0, s32 arg1);
s32 func_800226A4(void);
s32 func_800225B8(void);
void ovl_11_func_80100F9C(s32 arg0);
void func_800132F0(s32 arg0, s32 arg1, s32 arg2);
void ovl_11_func_8010289C(void);
void ovl_11_func_80100FFC(void);
void ovl_11_func_8010021C(void);
void ovl_11_func_80103714(void);

void ovl_11_func_801007AC(void) {
    s32 state;
    s16 choice;

    ovl_11_func_80100B50((u8)D_801273EC, 0);
    func_8002261C(3, 0x3BB);
    state = func_800226A4();
    if (state == 2) {
        choice = func_800225B8();
        if (choice == 1) {
            D_801273E6 = D_801273EC;
            ovl_11_func_80100F9C((u8)D_801273EC);
            func_800132F0(10, 0, 2);
            Ovl11_D_801273DC = ovl_11_func_8010289C;
        } else if (choice == 2) {
            ovl_11_func_80100F9C((u8)D_801273EC);
            Ovl11_D_801273DC = ovl_11_func_80100FFC;
        } else if (choice == 3) {
            Ovl11_D_801273DC = ovl_11_func_8010021C;
        } else {
            Ovl11_D_801273DC = ovl_11_func_80103714;
        }
    }
}
