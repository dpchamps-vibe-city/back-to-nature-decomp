#include "common.h"

void ovl_17_func_800B9FA8(void);
void ovl_17_func_800BA21C(void);
void ovl_17_func_800BA2C8(void);
void ovl_17_func_800BA504(void);
s32 func_8001FD74(void);
s32 func_80020818(void);
s32 func_8001FB30(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_17_func_800BA54C(void);
void func_800226F0(void);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
void func_8001FE00(s32 arg0);

void ovl_17_func_800B7E78(void) {
    switch (D_800BB524) {
    case 0:
        ovl_17_func_800B9FA8();
        D_800BB524++;
        break;
    case 1:
        ovl_17_func_800BA21C();
        D_800BB524++;
        break;
    case 2:
        ovl_17_func_800BA2C8();
        D_800BB524++;
        break;
    case 3:
        ovl_17_func_800BA504();
        D_800BB524++;
        break;
    case 4:
        if (func_8001FD74() != 0) {
            D_800BB524 = 5;
            func_80020818();
            func_8001FB30(0, 0, 0x15, 0x7F);
        }
        break;
    }
    if (D_800BB524 == 5) {
        D_800BD84C = 0;
        ovl_17_func_800BA54C();
        func_800226F0();
        func_800132B8(0xA, 0, 2);
        func_8001FE00(0xA);
        D_800BB524 = 0;
        ((Ovl17CompletionView *)D_8006C838)->state++;
    }
}
