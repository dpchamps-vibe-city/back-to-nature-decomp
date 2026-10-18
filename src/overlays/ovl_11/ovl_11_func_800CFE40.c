#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
s32 ovl_11_func_800D2950(s32 arg0);
void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                    s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
s32 func_8001DFD4(s32 *arg0, s16 *arg1);
s32 ovl_11_func_800D037C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_800158E4(SpriteSourceData *src);

void ovl_11_func_800CFE40(u8 *arg0) {
    s32 screen[4];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        D_80128D38[0] = *(u16 *)(arg0 + 0x38) + 0x96;
        D_80128D38[1] = *(u16 *)(arg0 + 0x3C) - 0x64;
        D_80128D38[2] = *(u16 *)(arg0 + 0x40) - 0x96;
        if (ovl_11_func_800F5888((u16 *)D_80128D38, screen) != 0) {
            ot = D_8005E3C0->field_120 + ((screen[2] >> 2) * 4);
            ovl_11_func_80107E2C(arg0 + 0xA8, ot, (s16)screen[0],
                (s16)(screen[1] + ovl_11_func_800D2950(*(s16 *)arg0)));
            func_80016054(ot, D_8005E3C0->field_118, (s32)(arg0 + 0x78),
                arg0[0x7C], arg0[0x7D], (s16)screen[0], (s16)screen[1],
                *(s32 *)(arg0 + 0x80), 0, 0x40);
            if (*(s32 *)(arg0 + 0x34) & 0x01000000) {
                D_80128D38[0] = *(u16 *)(arg0 + 0x38) + 0x96;
                D_80128D38[1] = -0x64;
                D_80128D38[2] = *(u16 *)(arg0 + 0x40) - 0x96;
                ot = D_8005E3C0->field_120 +
                    ((func_8001DFD4(D_80128D40, (s16 *)D_80128D38) >> 2) * 4);
            } else {
                D_80128D40[0] = screen[0];
                D_80128D40[1] = screen[1];
            }
            ovl_11_func_800D037C(*(s32 *)(arg0 + 0x34), ot,
                D_80128D40[0], D_80128D40[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            func_800158E4((SpriteSourceData *)(arg0 + 0x78));
        }
    }
}
