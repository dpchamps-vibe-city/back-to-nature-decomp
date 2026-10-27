#ifndef OVL11_RECORD_REMAP_TABLES_H
#define OVL11_RECORD_REMAP_TABLES_H

#include "common.h"

/* ovl_11:800F0C70. Apply four pair-assignment tables, in order:
 * 1. Seven F8-stride pairs at work+DEC4/DEC6 get {first,second-1}.
 * 2. Five indexed event records' +24/+26 pairs get their table values.
 * 3. Five conditional assignments run only where the event record pair
 *    matches the CURRENT work pair (+5246/+5248).
 * 4. The index--1-terminated override list unconditionally replaces pairs.
 * Later passes can overwrite earlier results. The sentinel's pair is never
 * read (the extracted final entry only contains its prefix).
 * No return value or range checks; all table indices and initialized work /
 * record storage must be valid. Pair values retain their original halfword
 * signedness/conversions; no calendar/gameplay interpretation is assumed. */
void Ovl11ApplyRecordRemapTables(void);

#endif
