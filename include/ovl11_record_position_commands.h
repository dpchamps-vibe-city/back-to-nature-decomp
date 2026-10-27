#ifndef OVL11_RECORD_POSITION_COMMANDS_H
#define OVL11_RECORD_POSITION_COMMANDS_H

#include "common.h"

/* Object-program opcodes 103/104. ID 0x29 selects gObjectWorkPosition;
 * every other signed ID indexes gObjectPositionRecordCoordinates at stride
 * 0x1D4 WITHOUT a range check. Callers must supply valid records/slot indices.
 * Positions are signed X/Y/Z words in raw record units. Both return 1.
 */

/* ovl_11:0x800EDD54. resolveFlags bit 0 resolves idOrSlot through a value
 * slot; bit 1 does the same for targetIdOrSlot. Resolved/direct IDs narrow
 * to signed halfwords. Face toward the target along the greater X/Z delta;
 * ties choose Z (coincident positions choose -Z). Primary ID 0x29 uses the
 * primary-object direction/update service, ignoring animation. Other IDs
 * pass animation's low halfword and the direction to the animation command,
 * then clear state-record flag 800. No direct coordinate store; the primary
 * path invokes an object update and inherits that service's state changes.
 */
s32 Ovl11FaceRecordTowardCommand(s16 idOrSlot, s16 resolveFlags,
                                s32 animation, s32 targetIdOrSlot);

/* ovl_11:0x800EDEB8. Add twice each delta, in Y/X/Z order. No slot lookup,
 * animation update, clipping or integration. Products/sums must fit s32.
 */
s32 Ovl11OffsetRecordPositionCommand(s16 recordId, s16 yDelta,
                                    s32 xDelta, s32 zDelta);

#endif
