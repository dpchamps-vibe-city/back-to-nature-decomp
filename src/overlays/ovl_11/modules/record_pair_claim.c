/* Claim the B4-pool half of the shared pair-slot table. The slot service
 * and small-record pool are independent providers, not owned by this unit. */
#include "common.h"
#include "ovl11_record_pair_claim.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_small_record_handlers.h"

s32 Ovl11ClaimRecordPairSlot(s32 pairIndex, s32 linkedRecordIndex) {
    s32 slot;
    struct Ovl11ResetB4 *record;

    slot = pairIndex + 2;
    if (Ovl11GetRecordPairSlotState(slot) != 0) {
        return 0x1E6;
    }
    record = (struct Ovl11ResetB4 *)Ovl11FindFreeB4Record();
    if (record != 0) {
        record->flags |= 0x02000000;
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][0] = Ovl11GetSmallRecordIndex(record);
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][1] = linkedRecordIndex;
        return 0;
    }
    return 0x1A1;
}
