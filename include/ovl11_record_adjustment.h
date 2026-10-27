#ifndef OVL11_RECORD_ADJUSTMENT_H
#define OVL11_RECORD_ADJUSTMENT_H

#include "common.h"

/* ovl_11:800F0734. Read a signed adjustment from [row][column]; valid indices
 * are rows 0..3 and columns 0..4, with no checking. A nonzero held-item ID
 * adds 2 for an even row or 20 for an odd row. If the record's resolved
 * halfword pair matches shared work +44BA/+44BC, multiply the result by 5.
 * Both bias and multiplication narrow back to s16, in that order.
 *
 * No record, value slot or table is modified. The record ID must resolve;
 * the pair-match helper's two output slots are disabled with -1. The sole
 * current caller selects rows 0/2 and adds the result to record +16 with an
 * upper cap of 255; the quantity's gameplay meaning/units remain unknown. */
s16 Ovl11LookupRecordAdjustment(s16 recordId, s32 row, s16 column);

#endif
