#ifndef OVL11_MESSAGE_COMMAND_H
#define OVL11_MESSAGE_COMMAND_H

#include "common.h"

/* Request/service a message through the shared message window. entryOrSlot is
 * a signed entry id, or a gOvl11ValueSlots index when entryIsSlot != 0.
 * For archive 0 and work+E4C8 == 2, nine mapped entries become 0x731..0x739.
 * Nonzero CD-audio state suppresses the request and returns 0 without exporting
 * a choice. Otherwise the request result is ignored; return 1 exactly when
 * the subsequent window status is 2, writing its choice to resultSlot unless
 * that slot is -1. Other statuses return 0. No slot bounds are checked.
 * This does not replace the scene's UpdateMessageWindow service.
 */
s32 Ovl11StepMessageCommand(s16 archive, s16 entryOrSlot, s32 resultSlot,
                           s32 entryIsSlot);

#endif
