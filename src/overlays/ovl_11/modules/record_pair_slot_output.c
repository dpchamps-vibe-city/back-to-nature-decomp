/* Resolve/export the two signed fields associated with a record. Outputs
 * are optional; the return reports equality with the current work pair,
 * NOT whether an output was written. See the public header and grouping note.
 */
#include "common.h"
#include "ovl11_record_pair_slot_output.h"

s32 Ovl11StoreRecordPairToSlots(s16 recordOrSlot, s16 source, s32 firstSlot, s32 secondSlot) {
    s16 second;

    /* Resolve in place: source begins as the slot-indirection flag, then
     * holds the selected record ID, and finally its first pair component. */
    if (source != 0) {
        source = gOvl11ValueSlots[recordOrSlot];
    } else {
        source = recordOrSlot;
    }
    if (source == 0x29) {
        source = ((Ovl11SpecialRecordPairView *)D_8006C838)->first;
        second = ((Ovl11SpecialRecordPairView *)D_8006C838)->second;
    } else if (source >= 0x32) {
        s16 index = (s16)(source - 0x32);
        char *base = (char *)D_8006C838;
        char *record = base + index * 0xF8;
        source = *(s16 *)(record + 0x8000 + 0x5EC4);
        second = *(s16 *)(record + 0x8000 + 0x5EC6);
    } else {
        char *base = (char *)D_8006C838;
        char *record = base + source * 0x1D4;
        source = *(s16 *)(record + 0x8000 + 0x1A0C);
        second = *(s16 *)(record + 0x8000 + 0x1A0E);
    }
    if (firstSlot != -1) {
        gOvl11ValueSlots[firstSlot] = source;
    }
    if (secondSlot != -1) {
        gOvl11ValueSlots[secondSlot] = second;
    }
    if (((Ovl11CurrentRecordPairView *)D_8006C838)->first == source) {
        if (((Ovl11CurrentRecordPairView *)D_8006C838)->second == second) {
            return 1;
        }
    }
    return 0;
}
