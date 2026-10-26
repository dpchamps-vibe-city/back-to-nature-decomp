/*
 * shared_animation_icon — reconstructed translation unit.
 *
 * Submit an explicitly selected frame of animation 24 from the shared sprite
 * source. Resets the source animation/frame state on each call; does not
 * advance it or present the GPU frame.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_shared_animation_icon.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "sprite_render.h"

void Ovl11DrawSharedAnimationIcon(s32 otAddress, s32 x, s32 y, s32 frame) {
    SelectSpriteAnimation((ObjectState *)&gSharedSpriteSource, 0x18);
    gSharedSpriteSource.field_5 = 0;
    DrawSpriteFrameRects(otAddress, (s32)&gSharedSpriteSource, 0x18,
                        frame & 0xFF, (s16)x, (s16)y);
}
