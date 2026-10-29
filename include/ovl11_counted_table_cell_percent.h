#ifndef OVL11_COUNTED_TABLE_CELL_PERCENT_H
#define OVL11_COUNTED_TABLE_CELL_PERCENT_H

#include "common.h"

/* ovl_11:0x800F00E4. Integer percentage, rounded down, of nonexcluded IDs
 * in the work area's 45-by-25 cell table. Flag 0x3E == 1 adds a 7-by-7
 * table and changes the denominator from 1125 to 1174. The 18 excluded IDs
 * are 0x3A..0x40, 0x167 and 0x16C..0x175. No table or flag is modified.
 */
s32 Ovl11GetCountedTableCellPercent(void);

#endif
