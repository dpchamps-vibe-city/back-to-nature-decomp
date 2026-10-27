/* Signed table adjustment with an item-dependent bias and a record/work
 * pair-match multiplier. The caller applies the result to record +16;
 * its gameplay meaning and units are not established. */
#include "common.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_record_adjustment.h"
#include "ovl11_record_pair_slot_output.h"

s16 Ovl11LookupRecordAdjustment(s16 recordId, s32 row, s16 column) {
    s16 adjustment;
    s16 biasedAdjustment;

    adjustment = gOvl11RecordAdjustmentRows[row][column];
    if (Ovl11GetRecordItemId((s16 *)&Ovl11Item_80071A8A) != 0) {
        if (row & 1) {
            biasedAdjustment = adjustment + 0x14;
        } else {
            biasedAdjustment = adjustment + 2;
        }
        adjustment = biasedAdjustment;
    }
    if (Ovl11StoreRecordPairToSlots(recordId, 0, -1, -1) != 0) {
        adjustment *= 5;
    }
    return adjustment;
}
