/*
 * dispatch_record_flag_rules — reconstructed translation unit.
 *
 * Apply flag-bit 20/40 rules to the enable bit of a caller-supplied
 * dispatch-record list.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_dispatch_record_flags.h"

void Ovl11ApplyDispatchRecordFlagRules(s16 count, Ovl11DispatchRecord *records) {
    Ovl11DispatchRecord *record;
    s32 index;

    record = records;
    for (index = 0; index < count; index++) {
        if (record->flags & 0x20) {
            record->flags = record->flags | 1;
        } else if (record->flags & 0x40) {
            record->flags = record->flags & 0xFFFE;
        }
        record += 1;
    }
}
