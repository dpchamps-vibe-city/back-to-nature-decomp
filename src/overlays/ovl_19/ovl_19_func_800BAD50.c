#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void ovl_19_func_800BAD50(void) {
    u8 *base;

    SelectSpriteAnimation((ObjectState *)&gOvl19UiSprite, 9);
    base = gOvl19UiSprite - 0x1A0;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl19UiSprite, base[0x1A4],
                  base[0x1A5], 0, 0);
}
