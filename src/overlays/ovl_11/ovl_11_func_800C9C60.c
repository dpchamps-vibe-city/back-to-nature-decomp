#include "common.h"

void ovl_11_func_800C9BB8(void);
void ovl_11_func_800CD0C0(s32 *arg0);

void ovl_11_func_800C9C60(void) {
    u8 *base;

    base = (u8 *)&gPrimaryObjectBase;
    D_80128C54 = 0;
    D_80128C4C_dispatch = 0;
    gOvl11PrimaryHeldButtons = 0;
    gOvl11PrimaryPressedButtons = 0;
    if (*(s32 *)(base - 0x51BC) & 0x8000) {
        *(s16 *)(base + 0x90) = 3;
        *(s16 *)(base + 0x92) = 2;
        *(s16 *)(base + 0x96) = 8;
        *(s16 *)(base + 0x98) = 2;
        *(s16 *)(base + 0x9C) = 0xD;
        *(s16 *)(base + 0x9E) = 2;
        *(s16 *)(base + 0xA2) = 0x12;
        *(s16 *)(base + 0xA4) = 2;
        *(s16 *)(base + 0xA8) = 0x17;
        *(s16 *)(base + 0xAA) = 2;
        *(s16 *)(base + 0xAC) = 0x64;
        *(s16 *)(base + 0xAE) = 0x22;
        *(s16 *)(base + 0xB2) = 0xA;
        *(s16 *)(base + 0x1E) = 2;
        *(s16 *)(base + 0x20) = 8;
    }
    ovl_11_func_800C9BB8();
    ovl_11_func_800CD0C0((s32 *)(base + 0x100));
    D_80128D30 = 0;
    D_80128C68 = 0;
    D_80128C6C = 0;
}
