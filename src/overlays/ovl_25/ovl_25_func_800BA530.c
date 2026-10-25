#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "packet_work.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"


extern ObjectState D_800C024C;

void ovl_25_func_800BA530(void) {
    ObjectState *temp_s0;

    SelectSpriteAnimation(&D_800C024C, 0);
    temp_s0 = &D_800C024C - 0x81;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32) &D_800C024C, (s32) (*(u8 *) ((u8 *) temp_s0 + 0x40C)), (s32) (*(u8 *) ((u8 *) temp_s0 + 0x40D)), (s16) ((s32) ((HWD0 + ((u32) HWD0 >> 0x1F)) << 0xF) >> 0x10), (s16) ((s32) ((VWD0 + ((u32) VWD0 >> 0x1F)) << 0xF) >> 0x10));
}
