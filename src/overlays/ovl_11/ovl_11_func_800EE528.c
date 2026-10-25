#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "screen_primitives.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

/* Callee prototypes as this caller TU saw them. The served callee is
 * declared with a u8 second parameter: the target's argument narrowing is a
 * bare `andi a1,v1,0xFF`, not the `sll/sra` pair an s8 parameter emits. */
u8 *ovl_11_func_800EFF04(s32 arg0, s32 arg1, s32 *arg2);

s32 ovl_11_func_800EE528(s16 arg0, s16 arg1) {
    s32 temp_v1;
    u8 *temp_v0;

    if ((u32) (arg1 & 0xFFFF) >= 0xAU) {
        return 0;
    }
    temp_v0 = ovl_11_func_800EFF04(0x24, (s32) arg0, NULL);
    temp_v1 = gOvl11ValueSlots[arg1];
    if (temp_v0[4] != temp_v1) {
        SelectSpriteAnimation((ObjectState *) temp_v0, temp_v1 & 0xFF);
    }
    DrawScreenTileRect((u32 *) (gActiveRenderContext->field_D8 + 0x10), 0, 0, 0x140, 0xF0, 0, 0);
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) temp_v0, 0x50, 0x3C);
    return 1;
}
