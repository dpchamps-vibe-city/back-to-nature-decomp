#include "common.h"
#include "screen_fade.h"

void ovl_19_func_800BB5D4(void);
void ovl_19_func_800BB848(void);
void ovl_19_func_800BB8F4(void);
void ovl_19_func_800BBB48(void);
void ovl_19_func_800BBCCC(void);
s32 func_8001FD74(void);
s32 func_80020818(void);
s32 func_8001FB30(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
void ovl_19_func_800B82B8(void);
void ovl_19_func_800BB318(void);
void func_800226F0(void);
void func_8001FE00(s32 arg0);

void ovl_19_func_800B7F34(void) {
    s32 *base;

    switch (D_800BCEE4) {
    case 0:
        ovl_19_func_800BB5D4();
        D_800BCEE4++;
        break;
    case 1:
        ovl_19_func_800BB848();
        D_800BCEE4++;
        break;
    case 2:
        ovl_19_func_800BB8F4();
        D_800BCEE4++;
        break;
    case 3:
        ovl_19_func_800BBB48();
        D_800BCEE4++;
        break;
    case 4:
        ovl_19_func_800BBCCC();
        D_800BCEE4++;
        break;
    case 5:
        if (func_8001FD74() != 0) {
            D_800BCEE4 = 6;
            func_80020818();
            func_8001FB30(0, 0, 0xF, 0x7F);
        }
        break;
    }
    if (D_800BCEE4 == 6) {
        ovl_19_func_800B82B8();
        ovl_19_func_800BB318();
        func_800226F0();
        BeginScreenFadeDecrease(0xA, 0, 2);
        func_8001FE00(0xA);
        D_800BCEE4 = 0;
        base = (s32 *)&D_8006C838;
        base[0x1123]++;
    }
}
