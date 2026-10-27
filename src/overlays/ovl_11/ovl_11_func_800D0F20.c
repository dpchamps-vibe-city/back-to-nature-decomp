#include "common.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_small_record_handlers.h"

s32 ovl_11_func_800D0F20(s32 arg0, s32 arg1) {
    s32 slot;
    struct Ovl11ResetB4 *record;

    slot = arg0 + 2;
    if (Ovl11GetRecordPairSlotState(slot) != 0) {
        return 0x1E6;
    }
    record = (struct Ovl11ResetB4 *)Ovl11FindFreeB4Record();
    if (record != 0) {
        record->flags |= 0x02000000;
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][0] = Ovl11GetSmallRecordIndex(record);
        ((struct Ovl11PoolResetView *)D_8006C838)->pairs_99C8[slot][1] = arg1;
        return 0;
    }
    return 0x1A1;
}
