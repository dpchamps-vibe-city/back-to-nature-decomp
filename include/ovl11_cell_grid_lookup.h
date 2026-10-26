#ifndef OVL11_CELL_GRID_LOOKUP_H
#define OVL11_CELL_GRID_LOOKUP_H

#include "common.h"

/* Resolve a grid from the +20/+24 work-area tables, transform its 13x13 byte
 * cells with transformMode's low byte, and search for tag+1 (u32 arithmetic).
 * recordIndex points to a signed table index; adjacentRecord adds another
 * 0x28-byte record offset. A signed -1 in that header's +0xE means absent.
 * Tables, indices and transformation mode must already be valid.
 *
 * Returns 0 with outputs unchanged when absent/not found, otherwise 1.
 * Outputs are signed CELL offsets from (6,6), not world/pixel coordinates.
 * Each matching row overwrites them: result is the FIRST matching column in
 * the LAST matching row. Outputs must be writable; no allocation or persistent
 * state changes. Only the scanner has corroborated external callers. */
s32 Ovl11FindCellGridTag(s16 *recordIndex, u32 tag, s16 transformMode,
                       s16 *outX, s16 *outY, s16 adjacentRecord);

/* Original ovl_11:800D5C3C-800D5D38. Read a signed type byte at +3 of
 * recordHeaders[recordIndex] (0x28-byte stride), then a 0x10-byte type row.
 * Type -1 returns -1 for fields +02/+04, but ZERO for +0E. Otherwise +02/+0E
 * are unsigned halfwords promoted to s32; +04 is a signed halfword.
 * No other negative type/index or out-of-range index is guarded. */
s32 Ovl11ReadRecordTypeProperty02(s16 recordIndex);
s32 Ovl11ReadRecordTypeProperty04(s16 recordIndex);
s32 Ovl11ReadRecordTypeProperty0E(s16 recordIndex);

/* Original ovl_11:800D5D38-800D5FA8. propertyIndex selects one of 18 signed
 * halfword indices at header+4; recordIndex is an unsigned row index.
 * An index of -1 is absent; other indices address 8-byte flag records.
 * Predicates return 0 for absence or a clear bit, otherwise 1. Value readers
 * return -1 for absence OR when none of flags E000 is set, otherwise the
 * unsigned halfword at +0/+2 promoted to s32. The flag record is not changed.
 * All table pointers must be initialized; no range checks are performed. */
s32 Ovl11RecordPropertyHasFlag8000(u16 recordIndex, u16 propertyIndex);
s32 Ovl11RecordPropertyHasFlag2000(u16 recordIndex, u16 propertyIndex);
s32 Ovl11RecordPropertyHasFlag4000(u16 recordIndex, u16 propertyIndex);
s32 Ovl11ReadRecordPropertyValue0(u16 recordIndex, u16 propertyIndex);
s32 Ovl11ReadRecordPropertyValue2(u16 recordIndex, u16 propertyIndex);
s32 Ovl11RecordPropertyHasFlag0800(u16 recordIndex, u16 propertyIndex);

#endif
