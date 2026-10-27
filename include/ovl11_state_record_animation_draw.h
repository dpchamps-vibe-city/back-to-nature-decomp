#ifndef OVL11_STATE_RECORD_ANIMATION_DRAW_H
#define OVL11_STATE_RECORD_ANIMATION_DRAW_H

#include "common.h"

/* animationSlot must be 0..9; otherwise return 0 without resolving or drawing.
 * Resolve descriptor 0x24 at unchecked recordIndex. If its current animation
 * byte differs from the full signed slot word, select the slot's low byte.
 * Emit a black 320x240 tile and draw/update the sprite at pixel (80,60), using
 * the active render context's OT offsets +0x10 and +8 respectively. Returns 1.
 * Requires initialized descriptor/sprite/render storage; resolution has the
 * breakpoint behavior documented by ovl11_state_field_access.h. */
s32 Ovl11DrawStateRecordAnimation(s16 recordIndex, s16 animationSlot);

#endif
