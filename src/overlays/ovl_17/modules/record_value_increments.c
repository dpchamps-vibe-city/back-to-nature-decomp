/*
 * Bounded increments of two adjacent shared-record halfwords.
 * The byte-range path compares a signed value but reloads it unsigned for
 * the addition; this is not a general-purpose clamp for negative inputs.
 * Membership and outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl17_record_value_increments.h"

void Ovl17RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    /* Keep the shared-work base and the two offsets separate: the target
     * forms the 0x8000 bias before accessing the record's halfword. */
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) =
            (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}

void Ovl17RaiseRecordHalfwordRangeValue(s16 recordIndex) {
    char *work;
    char *record;
    u16 value;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    value = *(u16 *)(record + 0x8000 + 0x19EC);
    if (value <= 0xFE0Au) {
        *(u16 *)(record + 0x8000 + 0x19EC) = value + 0x1F4;
    } else {
        *(u16 *)(record + 0x8000 + 0x19EC) = 0xFFFF;
    }
}
