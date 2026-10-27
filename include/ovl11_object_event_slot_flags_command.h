#ifndef OVL11_OBJECT_EVENT_SLOT_FLAGS_COMMAND_H
#define OVL11_OBJECT_EVENT_SLOT_FLAGS_COMMAND_H

#include "common.h"

/* ovl_11 must be resident. Clear flags at +0x1E of a 0x1D4-stride event slot:
 * mode != 2 clears 0x1800; mode != 0 also clears display-enable bit 0x100.
 * slot 42 broadcasts recursively to slots 1..36 and returns 1, ignoring
 * each recursive result. Other slots return 0 unless slot > 0 && mode < 37.
 * The upper-bound test is on MODE, not slot: callers must validate slot
 * storage themselves. Negative modes are accepted; mode >= 37 does nothing.
 * Returns 1 after an accepted operation, even if no bit changed. Does not
 * reset any other record fields. Last operands are unused except forwarding
 * on broadcast. Original identity: ovl_11:800E9620. */
s32 Ovl11ClearObjectEventSlotFlags(s16 slot, s16 mode, s32 unused2, s32 unused3);

#endif
