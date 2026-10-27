#ifndef OVL11_SPECIAL_COUNTER_SLOT_COMMAND_H
#define OVL11_SPECIAL_COUNTER_SLOT_COMMAND_H

#include "common.h"

/* ovl_11 must be resident. Choose among event slots {3,7,9,14,20} by the
 * greatest unsigned counter at record+4, randomly breaking ties (even a lone
 * maximum calls Rand). With skipFlagFilter == 0, game flags 0x21..0x24 exclude
 * the first four and predicate EFA1C(0) excludes slot 20. In either mode,
 * slot 20 must also pass the record-action predicate EDF3C.
 * Return 0 without a random choice/slot write if all candidates are excluded.
 * Otherwise return 1 and write the selected ID to gOvl11ValueSlots[resultSlot]
 * unless resultSlot == -1; that sentinel still consumes the random choice.
 * Slot index is signed and unchecked. Candidate data is copied, not mutated;
 * event counters are read-only. Original identity: ovl_11:800ED7E0. */
s32 Ovl11ChooseSpecialCounterSlotCommand(s16 skipFlagFilter, s16 resultSlot);

#endif
