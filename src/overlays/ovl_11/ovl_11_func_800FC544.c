#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

extern u8 D_8012CDF8[];

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FC544(s32 arg0, s16 arg1, s32 arg2, s32 arg3, s16 arg4) {
    s16 t = arg3;

    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32)(D_8012CDF8 + arg0 * 0x30), arg1 & 0xFF, 0, t, arg4);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, arg2, (s16)(t + 0x18), (s16)(arg4 + 4));
}
