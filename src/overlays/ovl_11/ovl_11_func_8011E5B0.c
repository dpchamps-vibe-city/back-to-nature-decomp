#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void func_800132F0(s32 arg0, s32 arg1, s32 arg2);
void func_800132B8(s32 arg0, s32 arg1, s32 arg2);
s32 func_80013394(void);
void func_80022580(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5);
void func_80015BF0(s32 arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3);
s32 GetVal8005E5B4(void);
u32 func_80017A64(void);
void func_80017A48(u32 arg0);
void ovl_11_func_8011EC84(void);
void ovl_11_func_800BCF28(void);
void ovl_11_func_801209A8(void);
void ovl_11_func_8011EA0C(void);

void ovl_11_func_8011E5B0(void) {
    u32 saved;

    if (D_80128420 == 1) {
        func_800132F0(10, 0, 2);
        D_80128420 = 2;
    } else if (D_80128420 == 2) {
        if (func_80013394() == 1) {
            D_80128420 = 3;
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x20000;
        }
    } else if (D_80128420 == 3) {
        if (D_8006C844 & 0x800000) {
            StoreImage((RECT *)D_80128524, (u_long *)(D_8005E3B0 + 0x12A90));
            DrawSync(0);
            D_80128420 = 4;
        }
    } else if (D_80128420 == 4) {
        ovl_11_func_8011EC84();
        D_80128420 = 5;
        func_800132B8(10, 0, 2);
    } else if (D_80128420 == 5) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x68), 1, 0, 0, 320, 176);
        func_80015BF0(D_8005E3C0->field_D8 + 0x64, (SpriteSourceData *)D_8012DA80, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (func_80013394() == 1) {
            D_80128420 = 6;
        }
    } else if (D_80128420 == 8) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x68), 1, 0, 0, 320, 176);
        func_80015BF0(D_8005E3C0->field_D8 + 0x64, (SpriteSourceData *)D_8012DA80, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (GetVal8005E5B4() == 2) {
            func_800132F0(10, 0, 2);
            D_80128420 = 9;
        }
    } else if (D_80128420 == 9) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x68), 1, 0, 0, 320, 176);
        func_80015BF0(D_8005E3C0->field_D8 + 0x64, (SpriteSourceData *)D_8012DA80, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (func_80013394() == 1) {
            LoadImage((RECT *)D_80128524, (u_long *)(D_8005E3B0 + 0x12A90));
            DrawSync(0);
            ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x40000;
            func_800132B8(10, 0, 2);
            D_80128420 = 10;
        }
    } else if (D_80128420 == 10) {
        if (func_80013394() == 1) {
            ovl_11_func_800BCF28();
            D_80128420 = 0;
        }
    } else if (D_80128420 == 11) {
        func_80022580((u32 *)(D_8005E3C0->field_D8 + 0x68), 1, 0, 0, 320, 176);
        func_80015BF0(D_8005E3C0->field_D8 + 0x64, (SpriteSourceData *)D_8012DA80, (s16)(HWD0 / 2), (s16)(VWD0 / 2));
        if (func_80013394() == 1) {
            D_80128420 = 12;
            ovl_11_func_801209A8();
        }
    } else if (D_80128420 == 12) {
        ovl_11_func_8011EC84();
        D_80128420 = 5;
    } else if (D_80128420 != 0) {
        saved = func_80017A64();
        func_80017A48(3);
        ovl_11_func_8011EA0C();
        func_80017A48(saved);
    }
}
