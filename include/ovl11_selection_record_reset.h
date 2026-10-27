#ifndef OVL11_SELECTION_RECORD_RESET_H
#define OVL11_SELECTION_RECORD_RESET_H

#include "common.h"

/* Clear ten text-address words (+0..+0x27), ten value halfwords
 * (+0x28..+0x3B) and ten row-type bytes (+0x3C..+0x45).
 * record must be word-aligned and writable for at least 0x46 bytes.
 * Does NOT initialize the selected row, column widths or other control
 * fields of the usual 0x60-byte selection record. No allocation or drawing;
 * all record ownership stays with the caller. ovl_11 must be resident. */
void Ovl11ClearSelectionRows(s32 *record);

#endif
