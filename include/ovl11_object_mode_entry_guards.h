#ifndef OVL11_OBJECT_MODE_ENTRY_GUARDS_H
#define OVL11_OBJECT_MODE_ENTRY_GUARDS_H

#include "common.h"
#include "game_types.h"

/* Sparse entry callbacks for modes 5, 6 and 13 in gOvl11ObjectModeEntryGuards.
 * They run BEFORE Ovl11SetObjectRecordMode commits the mode; -1 rejects it,
 * but does not roll back messages, values, flags, facing or effect changes.
 * Inputs must have the documented partial layout, initialized work/effect
 * pointers and valid +AC table index (0..7). None allocates storage.
 *
 * Mode 5 subtracts 3 from +16 through its existing unsigned-halfword write
 * and signed clamp sequence, clears +34 bit 800, starts effect 24 hex for
 * 30 update ticks, and rejects only when the object's area is 28 hex.
 * Mode 6 faces opposite the work direction, processes a held-item response
 * when present, then selects/request messages using grid/record eligibility,
 * seasonal counters and object flags. Its helper can change +16, flags and
 * message state before admission rejects. Mode 13 requests its indexed
 * message, sets +34 bit 2, clears work +5234 bit 400, and always rejects.
 * Counts are service calls, not seconds. */
s32 Ovl11GuardObjectMode5(Ovl11Mode5GuardObject *object);
s32 Ovl11GuardObjectMode6(Ovl11DE68Object *object);
s32 Ovl11GuardObjectMode13(u8 *object);

#endif
