#ifndef OVL30_DEBUG_VALUE_EDITORS_H
#define OVL30_DEBUG_VALUE_EDITORS_H

#include "common.h"

/* Debug page callbacks, slots 13..17 of ovl_30's menu-handler table. Each
 * returns handlerState unchanged; persistent cursors are separate globals.
 * Poll pad 0 before calling: input is repeatButtons (+0), NOT heldButtons
 * (+4) or pressedButtons (+8). Each call edits live shared data and emits
 * FntPrint text; it does not save, submit the font queue, or present a frame.
 * Reentry keeps cursor positions. Multiple simultaneous masks run in source
 * order, not as mutually exclusive choices. No undo/range validation exists.
 *
 * Byte pages: masks 2000/8000/4000/1000 move the cursor +10/-10/+1/-1.
 * Masks 20/40/10/80 edit the byte +1/-1/+10/-10. Arithmetic narrows to u8
 * before the >=255 reset; decrementing zero writes 255, other decrements
 * wrap naturally. This intentionally is not symmetric saturating arithmetic.
 * Ten rows starting at the cursor are printed, without wrapping the list.
 * Names follow the original debug labels, not an inferred gameplay meaning.
 */
s32 Ovl30EditDayCount0(s32 handlerState); /* 128 bytes at work+E640; 801326C4 */
s32 Ovl30EditDayCount1(s32 handlerState); /* 8 bytes at work+E6C0; 80132978 */
s32 Ovl30EditNValue(s32 handlerState);    /* 128 bytes at work+E6C8; 80132C2C */

/* Item index 0..378; field 0..3 is printed as BUY/SELL/LIFE/TIRED.
 * Masks 8/4 step the item index +/-10, 2000/8000 +/-1; 1000/4000 select
 * the field. Mask 80 subtracts one and 20 adds one, then the existing field
 * width truncates the result. No price/attribute semantic validation.
 * Original identity: ovl_30:0x80132F18. */
s32 Ovl30EditItemFields(s32 handlerState);

/* Select one of five tools with 2000/8000 and one of five signed-halfword
 * parameters with 1000/4000; edit it with 80/20 (-1/+1). This modifies the
 * shared 5x5 table at 801231C0, not overlay-owned storage. Parameter units
 * are not established by this debug screen. Original identity: 80133218. */
s32 Ovl30EditToolParameters(s32 handlerState);

#endif
