#ifndef OVL11_ELIGIBLE_RECORD_SELECTION_H
#define OVL11_ELIGIBLE_RECORD_SELECTION_H

#include "common.h"

/* Randomly choose an eligible record in area; return -1 if none qualify.
 * Area 5 always uses the 20-entry large pool. Area 4 uses the ten-entry
 * small pool; other areas use the large pool when mode has bits 1 or 2 set.
 *
 * Mode bits are additive eligibility sources:
 *   0x1: clear availability byte (area 4/small or area 5/large only).
 *   0x2: large record with flag 0x4 clear.
 *   0x4: large ID 0x162/0x165 meeting its flag/counter test.
 *   0x8: matching area from spawn-record kind 0x122, indices 0..9.
 * Record validity/area checks gate the pool tests, not the spawn-record test.
 * Matching kind-10 reservations can exclude any candidate; their flag bit 9
 * must differ from mode bit 0 to leave that candidate eligible.
 *
 * Only mode == 1 substitutes special indices 20/21 for the two tracked slots,
 * and omits either slot when its selected byte is nonzero. Ordinary results
 * are pool indices, not record IDs. Selection advances Rand's state; it does
 * not claim a slot or modify the records. Shared record/selection state must
 * already be initialized. Flags and counter units not established here retain
 * their numeric identities rather than speculative gameplay names.
 */
s32 Ovl11ChooseEligibleRecord(s16 area, u16 mode);

#endif
