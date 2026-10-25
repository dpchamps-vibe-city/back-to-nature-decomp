#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "screen_primitives.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"

s32 ovl_11_func_800EE528(s16 arg0, s16 arg1) {
    s32 temp_v1;
    u8 *temp_v0;

    if ((u32) (arg1 & 0xFFFF) >= 0xAU) {
        return 0;
    }
    temp_v0 = Ovl11ResolveStateField(0x24, (s32) arg0, NULL);
    temp_v1 = gOvl11ValueSlots[arg1];
    if (temp_v0[4] != temp_v1) {
        SelectSpriteAnimation((ObjectState *) temp_v0, temp_v1 & 0xFF);
    }
    DrawScreenTileRect((u32 *) (gActiveRenderContext->field_D8 + 0x10), 0, 0, 0x140, 0xF0, 0, 0);
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) temp_v0, 0x50, 0x3C);
    return 1;
}
