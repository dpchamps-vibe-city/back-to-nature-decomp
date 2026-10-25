#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

s32 ovl_11_func_800F5888(u16 *arg0, s32 *arg1);
void ovl_11_func_80107E2C(u8 *arg0, s32 arg1, s16 arg2, s16 arg3);
void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
s32 ovl_11_func_800D037C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800D001C(u8 *arg0) {
    u16 position[3];
    s32 projected[3];
    s32 ot;

    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x800000)) {
        position[0] = *(u16 *)(arg0 + 0x38) + 0x96;
        position[1] = *(u16 *)(arg0 + 0x3C) - 0x64;
        position[2] = *(u16 *)(arg0 + 0x40) - 0x96;
        if (ovl_11_func_800F5888(position, projected) != 0) {
            ot = gActiveRenderContext->field_120 + ((projected[2] >> 2) * 4);
            ovl_11_func_80107E2C(arg0 + 0xA8, ot, (s16)projected[0], (s16)(projected[1] - 0x14));
            if (*(u16 *)(arg0 + 0xB6) != 0) {
                DrawSpriteFrameRects(ot, (s32)(arg0 + 0xC8), 0, 0, (s16)projected[0], (s16)(projected[1] - 0x10));
            }
            func_80016054(ot, gActiveRenderContext->field_118, (s32)(arg0 + 0x78), arg0[0x7C], arg0[0x7D], (s16)projected[0], (s16)projected[1], *(s32 *)(arg0 + 0x80), 0, 0x40);
            ovl_11_func_800D037C(*(s32 *)(arg0 + 0x34), ot, projected[0], projected[1]);
        }
        if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
            UpdateSpriteAnimation((SpriteSourceData *)(arg0 + 0x78));
        }
    }
}
