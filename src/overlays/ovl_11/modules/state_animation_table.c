/* State-object sprite selection table, separate from state transitions.
 * The caller supplies the object's +24 state and +22 variant, then applies
 * these two outputs through the sprite-animation API. Original entry:
 * ovl_11:800D3424. Membership/boundaries: notes/file-groupings.md. */
#include "ovl11_state_animation_table.h"

s32 Ovl11ReadStateSpriteAnimation(u16 state, u16 variant,
                                u16 *animation, u16 *frame) {
    *animation = gOvl11StateSpriteFrames[state][variant].animation;
    if (frame != 0) {
        *frame = gOvl11StateSpriteFrames[state][variant].frame;
    }
    return 0;
}
