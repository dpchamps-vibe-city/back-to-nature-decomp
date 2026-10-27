/* Fixed three-position placement for code-0x109 small records.
 * The private table has 16-byte rows; only X/Y/Z are copied. Keep the
 * independent Y pointer and row stride rather than shrinking its padding.
 * Original entry: ovl_11:800CFD9C; evidence: notes/file-groupings.md. */
#include "ovl11_code109_record_placement.h"
#include "ovl11_small_record_handlers.h"

void Ovl11PlaceCode109Records(void) {
    Ovl11Code109Position *position;
    s16 *record;
    s32 index;
    s16 parameter;
    s32 *y;

    index = 0;
    parameter = 0x30;
    position = gOvl11Code109Positions;
    y = (s32 *)((u8 *) gOvl11Code109Positions + 4);
    do {
        record = Ovl11AllocateSmallRecord(0x109, -1, 0);
        if (record != 0) {
            (*(s16 *) ((u8 *) record + 0x30)) = parameter;
            (*(s16 *) ((u8 *) record + 0x22)) = index;
            (*(s32 *) ((u8 *) record + 0x38)) = position->x;
            (*(s32 *) ((u8 *) record + 0x3C)) = *y;
            (*(s32 *) ((u8 *) record + 0x40)) = position->z;
        }
        position = (Ovl11Code109Position *) ((u8 *) position + 0x10);
        index += 1;
        y += 4;
    } while (index < 3);
}
