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
 * state changes. */
s32 Ovl11FindCellGridTag(s16 *recordIndex, u32 tag, s16 transformMode,
                       s16 *outX, s16 *outY, s16 adjacentRecord);

/* Original ovl_11:800D5C3C-800D5D38. Read a signed type byte at +3 of
 * recordHeaders[recordIndex] (0x28-byte stride), then a 0x10-byte type row.
 * Type -1 returns -1 for fields +00/+02/+04, but ZERO for +0E. Otherwise
 * +00/+02/+0E are unsigned halfwords promoted to s32; +04 is signed.
 * No other negative type/index or out-of-range index is guarded. */
s32 Ovl11ReadRecordTypeProperty00(s16 recordIndex); /* ovl_11:800D61D8 */
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

/* Item table entries are 0x28 bytes. Prices (+10) and sell values (+12) are
 * signed halfwords promoted to s32; callers use balance units. Sell lookup
 * masks to 16 bits and adds 10 only for item 0x97 with state flag 0x400000.
 * Neither lookup checks the index or clamps the result. */
s32 Ovl11GetItemPrice(u16 itemIndex);
s32 Ovl11GetItemSellValue(s32 itemIndex);

/* Return 1 when item flags & 0x8100 == 0x8000 AND its type byte is not 1,
 * otherwise 0. Used by held-item selection; no state changes. */
s32 Ovl11ClassifyHeldItemType(u16 itemIndex);

/* A signed halfword record tagged 0xA4 carries an item id in record[1].
 * Other tags return 0. The borrowed record must have both halfwords. */
s32 Ovl11GetRecordItemId(s16 *record);

/* Signed item/property indices are unchecked. Item header +E selects a
 * 16-byte packed row (four words, ten 3-bit values per word). A -1 row
 * returns -1; otherwise item flag 0x4000 returns -2. Normally returns 0..7.
 * With flags & 0x180, values 0..2 become 5 and 3..4 become 6. No writes. */
s32 Ovl11GetItemPackedProperty(s16 itemIndex, s16 propertyIndex);

#endif
