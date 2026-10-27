/*
 * Resolved state-record animation display, ovl_11:800EE528–800EE604.
 * This command uses descriptor 0x24; it owns no sprite or value-slot storage.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "screen_primitives.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_record_animation_draw.h"

s32 Ovl11DrawStateRecordAnimation(s16 recordIndex, s16 animationSlot) {
    s32 animationId;
    u8 *sprite;

    if ((u32) (animationSlot & 0xFFFF) >= 0xAU) {
        return 0;
    }
    sprite = Ovl11ResolveStateField(0x24, (s32) recordIndex, NULL);
    animationId = gOvl11ValueSlots[animationSlot];
    /* Compare the full slot word, but selection consumes only its low byte. */
    if (sprite[4] != animationId) {
        SelectSpriteAnimation((ObjectState *) sprite, animationId & 0xFF);
    }
    DrawScreenTileRect((u32 *) (gActiveRenderContext->field_D8 + 0x10), 0, 0, 0x140, 0xF0, 0, 0);
    DrawSpriteAnimationQuadsAndUpdate(gActiveRenderContext->field_D8 + 8, (SpriteSourceData *) sprite, 0x50, 0x3C);
    return 1;
}
