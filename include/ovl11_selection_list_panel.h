#ifndef OVL11_SELECTION_LIST_PANEL_H
#define OVL11_SELECTION_LIST_PANEL_H

#include "common.h"

/* ovl_11 must be resident. Draw at pixel origin (x,y), using the current
 * render context. record is a word-aligned 0x60-byte selection record:
 * ten text pointers at +0, ten u16 values at +0x28, ten row-type bytes at
 * +0x3C, column bytes at +0x5A/+0x5B and selected s16 row at +0x5C.
 * No record writes. Formats into gOvl11TextFormatScratch and emits draw packets;
 * not reentrant. Live rows have 14-pixel pitch; panel height is 14*N+6,
 * width is 8*record[byte 0x5B]+15. The number column is the absolute X
 * 8*record[byte 0x5A]+28, not relative to x. Column span must allow the
 * formatter's 2- or 7-column deduction. There is no bounds validation. */
void Ovl11DrawSelectionListPanel(s32 *record, s16 x, s16 y);

#endif
