#ifndef OVL11_TRIPLE_RECORD_TABLE_H
#define OVL11_TRIPLE_RECORD_TABLE_H

#include "common.h"
#include "game_types.h"

/* Stable in-place compaction of count six-byte records. key == 0 means empty;
 * moved-from entries have all three halfwords cleared. Returns zero.
 * A nonpositive count does nothing. No storage is allocated.
 * Original identity: ovl_11:800CADBC. */
s32 Ovl11CompactTripleRecords(Ovl11TripleRecord *records, s32 count);

/* Exchange external with a table endpoint, compacting afterward. Direction 0
 * removes the first entry and inserts external at the tail; direction 1 removes
 * the last nonempty entry and prepends external. The removed entry replaces
 * external. Other directions do nothing. Always returns -1.
 *
 * count MUST be positive; external must not alias the table. For direction 1,
 * if either first/external key is nonzero, the table MUST contain a nonempty
 * entry: an empty table with nonempty external otherwise indexes records[-1].
 * Both keys zero makes no change. These are unchecked caller obligations.
 * Original identity: ovl_11:800CAE64. */
s32 Ovl11ExchangeTripleRecordEndpoint(Ovl11TripleRecord *records, s32 count,
                                    Ovl11TripleRecord *external, s32 direction);

#endif
