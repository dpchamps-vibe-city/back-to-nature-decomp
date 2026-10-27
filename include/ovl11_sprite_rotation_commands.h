#ifndef OVL11_SPRITE_ROTATION_COMMANDS_H
#define OVL11_SPRITE_ROTATION_COMMANDS_H

#include "common.h"

/* Script command over the sprite_rotation service. A nonzero fromValueSlot
 * resolves the SIGNED LOW HALFWORD of unchecked gOvl11ValueSlots[idOrSlot].
 * operation: 0 requests direction's low halfword; 1 tests IDLE (1), 2 HOLD
 * (3), 3 OUTBOUND (2), 4 tests state other than FREE/IDLE (0/1). Other
 * operations return 1. Request mode returns 1 regardless of request result.
 * Mode 4 retains two state lookups when the first is nonzero.
 *
 * Requires registered, valid sprite-rotation sources. Request mode forwards
 * a NULL result pointer to 80107C80: on that helper's unmatched-ID path,
 * id 0x20 with game flag 0x76 == 1 writes the special result through NULL. */
s32 Ovl11SpriteRotationCommand(s16 idOrSlot, s16 operation, s32 direction,
                              s32 fromValueSlot);

/* Legacy raw-ID mode-4 call whose Boolean result is deliberately discarded.
 * Does not stop, wait for, or advance a rotation. Kept as its own callback. */
void Ovl11DiscardSpriteRotationActiveQuery(s16 id);

#endif
