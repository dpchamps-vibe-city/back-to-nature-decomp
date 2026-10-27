/*
 * pair_slot_record_refresh — reconstructed translation unit.
 *
 * Refresh the two pair-slot dispatch records from slot states and the
 * stride-four selector halfwords.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_pair_slot_records.h"
#include "ovl11_record_pair_slots.h"

void Ovl11RefreshPairSlotRecords(void) {
    u8 *work;
    s16 *kindSelectors;
    s16 alternateKind;
    s32 slot;

    slot = 2;
    work = (u8 *) D_8006C838;
    kindSelectors = (s16 *) (work + 0x99D2);
    do {
        /* Only exactly 1 selects the alternate kind in this refresh path;
         * the lower-level writer accepts any nonzero selector. */
        alternateKind = *kindSelectors;
        if (alternateKind != 1) {
            alternateKind = 0;
        }
        Ovl11UpdatePairSlotRecord(slot - 2, Ovl11GetRecordPairSlotState(slot) == 1, alternateKind);
        slot += 1;
        kindSelectors += 2;
    } while (slot < 4);
}
