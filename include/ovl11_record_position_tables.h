#ifndef OVL11_RECORD_POSITION_TABLES_H
#define OVL11_RECORD_POSITION_TABLES_H

#include "common.h"
#include "game_types.h"

/* Write three raw world-coordinate words, leaving any fourth word untouched.
 * Z600 accepts slots 0..19 plus special slots 20/21 (rows 4/5, column zero).
 * X400 uses signed slot/5 and slot%5 without special cases. Neither validates
 * the resulting table index; callers must supply a valid slot. */
void Ovl11GetSlotPositionZ600(s32 *position, s16 slot);
void Ovl11GetSlotPositionX400(s32 *position, s16 slot);

/* Clear item id at +B6 and embedded sprite-source header at +DC. Does not
 * release storage or reset the rest of the source/record. */
void Ovl11ClearRecordItemSprite(u8 *record);

/* Copy the indexed position/facing into the record and set kind to 0x28.
 * The unsigned table index at +AC is unchecked; it must select a valid row. */
void Ovl11PlaceRecordFromTable(Ovl11TablePositionObject *record);

/* Select the archive/entry passed by the caller to RequestMessageWindow.
 * Always writes archive=0, then a table-indexed entry with flag-8 adjustment.
 * Kind 0x28 uses season/day; other kinds use +26, flags +B8 and signed
 * +AE/B0/B2 values. The flagged kind-2 path also sets +2C=300, clears +24
 * and flag 0x800 at +34. Does not open a window itself. */
void Ovl11SelectRecordMessage(Ovl11DE68Object *record, s32 *archive, s32 *entry);

/* Movement-period input to 800D12B8: kinds 8/9/10 at +26 choose the UNSIGNED
 * +AE/B0/B2 value, divide it by 52 and double the indexed work-area halfword
 * at +99DA. Other kinds return 24. Indices and the resulting period are not
 * checked. The existing word-sized address ABI is retained. */
s32 Ovl11GetRecordMovePeriod(s32 recordAddress);

#endif
