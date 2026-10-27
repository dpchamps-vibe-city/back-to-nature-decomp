#ifndef OVL11_PRIMARY_INPUT_OVERRIDE_COMMAND_H
#define OVL11_PRIMARY_INPUT_OVERRIDE_COMMAND_H

#include "common.h"

/* ovl_11 primary input-override adapter. mode 1 sets work+0x5234 bit 0x800000,
 * attempts the shared begin operation and returns 1 even if its guard declines.
 * Duration = ticks/2 (signed truncation), or 12 for ticks == 0; units are
 * qualifying object updates, not clock minutes. mode 2 first selects direction
 * from the dominant absolute X/Z of gObjectWorkPosition, then does mode 1:
 * X wins only strictly; +X/-X -> 3/1, +Z/nonpositive Z -> 2/0.
 * mode -1 clears work+0x51F6 and flags 0x800008, returning 1.
 * Other modes only poll, returning 1 iff primary-object flag 8 is clear.
 * No local timer advance. Initialize shared object/work state; position axes
 * used in abs must not be INT_MIN. Original identity: ovl_11:800ED634. */
s32 Ovl11PrimaryInputOverrideCommand(s16 mode, s16 ticks);

#endif
