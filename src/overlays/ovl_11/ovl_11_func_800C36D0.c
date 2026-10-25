#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
s32 func_8001AF44(u32 arg0);
void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                    s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
void ovl_11_func_800C9E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800C36D0(Ovl11AnimationObject *arg0) {
    u16 pos[3];
    s32 screen[4];
    s32 ot;

    pos[0] = *(u16 *)((u8 *)arg0 + 0x30) + 0x96;
    pos[1] = *(u16 *)((u8 *)arg0 + 0x34) - 0x64;
    pos[2] = *(u16 *)((u8 *)arg0 + 0x38) - 0x96;
    if (ovl_11_func_800F5888(pos, screen) != 0) {
        ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
        if (arg0->id == ((struct Ovl11FlagDateView *)D_8006C838)->field_E778) {
            if (func_8001AF44(0x72) != 0) {
                ot -= 0x14;
            }
        }
        ovl_11_func_80107E2C((u8 *)arg0 + 0xE0, ot, (s16)screen[0],
                              (s16)(screen[1] - 0x28));
        func_80016054(ot, gActiveRenderContext->field_118, (s32)&arg0->sprite,
                       arg0->sprite.field_4, arg0->sprite.field_5,
                       (s16)screen[0], (s16)screen[1], arg0->sprite.field_8,
                       0, 0x40);
        ovl_11_func_800C9E90(ot, screen[0], screen[1], 0);
    }
    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
        UpdateSpriteAnimation(&arg0->sprite);
    }
}
