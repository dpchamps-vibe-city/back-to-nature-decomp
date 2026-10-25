#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_render.h"

extern u8 D_80127358[];
extern s32 D_8012CE28;
extern s32 gTextBankOffsets[4];
extern s32 D_80053604;

void func_80017B3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800FD194(s32 arg0) {
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 0x68, (s32) &D_8012CE28, D_80127358[arg0], 0, 0xBA, 0x14);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, *gTextBankOffsets + (s32) &D_80053604, 0xD2, 0x1A);
}
