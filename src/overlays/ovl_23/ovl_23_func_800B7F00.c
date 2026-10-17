#include "common.h"

void ovl_23_func_800BB214 (void);
void ovl_23_func_800BB488 (void);
void ovl_23_func_800BB534 (void);
void ovl_23_func_800BB758 (void);
s32 func_8001FD74 (void);
s32 func_80020818 (void);
s32 func_8001FB30 (s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_23_func_800B81AC (void);
void func_800226F0 (void);
void func_800132B8 (s32 arg0, s32 arg1, s32 arg2);
void func_8001FE00 (s32 arg0);

void ovl_23_func_800B7F00(void) {
    s32 *base;

    switch (*(u8 *)&D_800BBA3C) {
    case 0:
        ovl_23_func_800BB214();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 1:
        ovl_23_func_800BB488();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 2:
        ovl_23_func_800BB534();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 3:
        ovl_23_func_800BB758();
        ++*(u8 *)&D_800BBA3C;
        break;
    case 4:
        if (func_8001FD74() != 0) {
            *(u8 *)&D_800BBA3C = 5;
            func_80020818();
            func_8001FB30(0, 0, 0x12, 0x7F);
        }
        break;
    }
    if (*(u8 *)&D_800BBA3C == 5) {
        ovl_23_func_800B81AC();
        func_800226F0();
        func_800132B8(0xA, 0, 2);
        func_8001FE00(0xA);
        *(u8 *)&D_800BBA3C = 0;
        base = (s32 *)&D_8006C838;
        ++base[0x448C >> 2];
    }
}
