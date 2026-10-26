#ifndef OVL11_TAGGED_RECORD_CHOICE_H
#define OVL11_TAGGED_RECORD_CHOICE_H

#include "common.h"

/* Original ovl_11:800D3230. Return a random eligible family-local index,
 * or -1 when none exists. Family 0 selects the 20-record large bank;
 * nonzero selects the 10-record small bank. Eligibility and RNG effects
 * are those of Ovl11ChooseEligibleTaggedRecord (ovl11_record_selection.h). */
s32 Ovl11ChooseTaggedRecordIndex(u16 family);

/* Original ovl_11:800D3250. recordCode must point to a readable halfword.
 * Return -1 unless it is 0x15B; otherwise return 0, even with no adjustment.
 * While clock advancement is active, work flag +44F8 bit 24 is set and the
 * signed work halfword +0x44C0 is >= 7, select from the large bank, falling
 * back to the small bank only on selection failure. Index 0 is NOT updated.
 * For index > 0, store (oldValue - 10 + signedScale / 50) modulo 65536
 * at record +0x16. Scale is the shared signed halfword at 80075A90 (large)
 * or work +0x9258 (small), not a field of the selected record. No clamping.
 * A selection can consume RNG even if it produces no write. */
s32 Ovl11ApplyTaggedRecordValueAdjustment(u16 *recordCode);

#endif
