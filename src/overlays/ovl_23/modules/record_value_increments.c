/*
 * Bounded increments of adjacent halfwords in indexed 0x1D4-byte work
 * records. B87F8 calls both services for its two lists of record indices.
 * The gameplay meaning of the values is not established.
 *
 * Original ovl_23 entries BB7A0/BB800 end the code segment. The preceding
 * audio setup shares no record state or helper. Membership/boundary evidence
 * is in notes/file-groupings.md; the ovl_25 twins are separate identities.
 */

#include "common.h"
#include "ovl23_record_value_increments.h"

void Ovl23RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) = (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}

void Ovl23RaiseRecordHalfwordRangeValue(s16 recordIndex) {
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
