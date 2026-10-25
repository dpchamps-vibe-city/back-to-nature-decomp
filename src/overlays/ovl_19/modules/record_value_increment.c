/*
 * Raise one shared-record halfword by 20, capping valid byte-range values
 * at 255. The signed comparison and unsigned reload are intentional; this
 * is not a general-purpose clamp for negative inputs.
 * Membership and outer boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "ovl19_record_value_increment.h"

void Ovl19RaiseRecordByteRangeValue(s16 recordIndex) {
    char *work;
    char *record;

    work = (char *)&D_8006C838;
    record = work + recordIndex * 0x1D4;
    /* Retain the shared-work base and the separate 0x8000 bias, as in the
     * original indexed addressing. */
    if (*(s16 *)(record + 0x8000 + 0x19EA) < 0xEB) {
        *(s16 *)(record + 0x8000 + 0x19EA) =
            (s16)(0x14 + *(u16 *)(record + 0x8000 + 0x19EA));
    } else {
        *(s16 *)(record + 0x8000 + 0x19EA) = 0xFF;
    }
}
