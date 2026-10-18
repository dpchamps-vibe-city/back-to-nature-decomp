#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
s32 func_80015DD4(s32 *arg0, SpriteSourceData *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);
s32 ovl_11_func_800D2950(s32 arg0);
void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3);
s32 func_8001DFD4(s32 *arg0, SVECTOR *arg1);
s32 ovl_11_func_800D037C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800158E4(SpriteSourceData *src);

void ovl_11_func_800D0198(u8 *arg0, s32 arg1, s32 arg2) {
    s32 screen[4];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        D_80128D50[0] = *(u16 *)(arg0 + 0x38) + 0x96;
        D_80128D50[1] = *(u16 *)(arg0 + 0x3C) - 0x64;
        D_80128D50[2] = *(u16 *)(arg0 + 0x40) - 0x96;
        if (ovl_11_func_800F5888((u16 *)D_80128D50, screen) != 0) {
            ot = D_8005E3C0->field_120 + ((screen[2] >> 2) * 4);
            func_80015DD4((s32 *)ot, (SpriteSourceData *)(arg0 + 0x78), (s16)screen[0], (s16)screen[1], 0x1000, 0, 0, D_8005E3BC + arg2, arg1);
            ovl_11_func_80107E2C(arg0 + 0xA8, ot, (s16)screen[0],
                (s16)(screen[1] + ovl_11_func_800D2950(*(s16 *)arg0)));
            if (*(s32 *)(arg0 + 0x34) & 0x01000000) {
                D_80128D50[0] = *(u16 *)(arg0 + 0x38) + 0x96;
                D_80128D50[1] = -0x64;
                D_80128D50[2] = *(u16 *)(arg0 + 0x40) - 0x96;
                ot = D_8005E3C0->field_120 +
                    ((func_8001DFD4(D_80128D48, (SVECTOR *)D_80128D50) >> 2) * 4);
            } else {
                D_80128D48[0] = screen[0];
                D_80128D48[1] = screen[1];
            }
            ovl_11_func_800D037C(*(s32 *)(arg0 + 0x34), ot,
                D_80128D48[0], D_80128D48[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            func_800158E4((SpriteSourceData *)(arg0 + 0x78));
        }
    }
}
