/*
 * event_record_flag_reset — reconstructed translation unit.
 *
 * Clear the object-load flag pair across all 37 event records without
 * resetting the records.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_object_load_resets.h"

void Ovl11ClearEventRecordLoadFlags(void) {
    s32 remaining;
    struct_80076220 *record;

    record = &gObjectEventSlotBase;
    remaining = 0x24;
    do {
        record->unk1E &= ~0x1800;
        remaining--;
        record++;
    } while (remaining >= 0);
}
