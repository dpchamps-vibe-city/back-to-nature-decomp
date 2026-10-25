/*
 * Two adjacent halfword values in 0x1D4-byte work records. The common
 * caller B9208 applies these increments to two lists of record indices.
 * The different caps/step sizes are clear; the gameplay meaning is not.
 *
 * Original ovl_25 entries BB970/BB9D0; the preceding animation-slot code
 * and following background-color code have separate state and protocols.
 * See notes/file-groupings.md for membership and boundary evidence.
 */

#include "common.h"
#include "ovl25_record_value_increments.h"

void Ovl25RaiseRecordByteRangeValue(s16 recordIndex) {
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

void Ovl25RaiseRecordHalfwordRangeValue(s16 recordIndex) {
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
