/*
 * display_sprite_frame — reconstructed translation unit.
 *
 * Select animation 4 on the scene display sprite and draw its current
 * rectangular frame at screen origin.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl17_display_sprite_frame.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void Ovl17DrawDisplaySpriteFrameRects(void) {
    u8 *workBase;

    SelectSpriteAnimation((ObjectState *)&gOvl17DisplaySpriteSource, 4);
    workBase = gOvl17DisplaySpriteSource - 0x22C;
    DrawSpriteFrameRects(gActiveRenderContext->field_D8 + 4, (s32)&gOvl17DisplaySpriteSource, workBase[0x230],
                  workBase[0x231], 0, 0);
}
