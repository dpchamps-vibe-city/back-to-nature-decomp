#ifndef OVL21_SCENE_SPRITE_DRAW_H
#define OVL21_SCENE_SPRITE_DRAW_H

#include "common.h"
#include "game_types.h"

/* Per-frame draw callbacks. Record: signed selectors at +2/+8, position at
 * +A/+C/+E, source pointer at +20. Slot: selector at +4, position at +C,
 * embedded source at +1C. Keep the historical word-pointer ABI and load widths.
 * A changed animation resets frame/timer; an unchanged animation is retained.
 * Both call the projected service below (draw THEN advance animation). */
void Ovl21DrawRecordSprite(s32 *record);
void Ovl21DrawSlotSprite(s32 *slot);

/* offsetIndex is unchecked 0..5 into the shared X/Z halfword-pair table.
 * Draw the primary scene source at {x,-500,z}, using animation 14. */
void Ovl21DrawPrimarySceneSprite(s16 offsetIndex);

/* Reset the fixed rect source to animation/frame 0 and draw at the screen
 * center through field_D8+4. Does not advance animation. */
void Ovl21DrawCenteredFrameRects(void);

/* Position words are interpreted through the original unsigned-halfword ABI,
 * then narrowed to the signed projection vector after adding {150,-100,-150}.
 * Project into the active field_120 OT, build quads, then advance animation.
 * Sources matching scene records 0/1/2/5 receive an additional screen pass.
 * Requires initialized, retained sprite headers, valid GTE/render state and
 * sufficient packet storage. Shared projection scratch makes this non-reentrant;
 * there is no clipping/capacity validation or GPU submission here. */
void Ovl21SubmitProjectedSprite(SpriteSourceData *source, u16 *position);

#endif
