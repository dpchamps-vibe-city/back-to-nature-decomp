/* Clear the ten row payloads of a caller-owned selection record.
 * Layout/width bytes, cursor and the record's other control fields survive.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_selection_record_reset.h"

void Ovl11ClearSelectionRows(s32 *rowText) {
    s32 row;
    s16 *rowValue;
    s8 *rowType;

    row = 0;
    rowType = (s8 *)rowText + 0x3C;
    rowValue = (s16 *)((s8 *)rowText + 0x28);
    do {
        *rowText = 0;
        *rowValue = 0;
        rowValue++;
        rowText++;
        rowType[row] = 0;
    } while (++row < 10);
}
