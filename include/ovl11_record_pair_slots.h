#ifndef OVL11_RECORD_PAIR_SLOTS_H
#define OVL11_RECORD_PAIR_SLOTS_H

#include "common.h"
#include "game_types.h"

/* Four shared selection slots at work+0x99C8. Slots 0/1 belong to the
 * B8-stride tagged pool; slots 2/3 to the B4-stride pool. Each pool's
 * expansion halfword enables both its second slot and second record half.
 * A poolKind of 1 selects B8; every other value selects B4. No index checks. */
void Ovl11ApplyRecordRowPlacement(s32 record, s16 rowSlot, s32 poolKind);
/* Re-place live records in mode 5 (B8) or 4 (B4), then place the two
 * selected B8 source records at row slots -1/-2 in either case. */
void Ovl11ResyncRecordRowPlacements(s32 poolKind);
/* Finds, but does not claim, the first enabled slot whose recordIndex is -1.
 * Returns null if none; callers must set the pair to claim the slot. */
Ovl11RecordPairSlot *Ovl11FindFreeRecordPairSlot(s32 poolKind);
/* Reconstruct selected records from the saved work-area selections.
 * B8 selects a copy constructor from the source flags; B4 recreates type
 * 0x10A at fixed positions, preserving the per-slot 0x40000000 flag choice. */
void Ovl11RestorePairedRecords(s32 poolKind);
/* Valid slots are 0..3: returns 0 free, 1 occupied, 2 disabled. */
s32 Ovl11GetRecordPairSlotState(s32 slot);

/* Find a zero-type record with flag 0x02000000 clear; no mutation/reservation.
 * B8 scans 10/20 records, B4 scans 5/10. Null means no available record. */
s32 *Ovl11FindFreeTaggedRecord(void);
s32 *Ovl11FindFreeB4Record(void);
/* Exact base-pointer search across ALL 20 tagged records; returns -1 if
 * the pointer is not an entry start, regardless of the expansion flag. */
s32 Ovl11GetTaggedRecordIndex(u8 *record);
void Ovl11SetRecordPairSlot(Ovl11RecordPairSlot *slot, s16 recordIndex, s16 linkedRecordIndex);
void Ovl11ClearRecordPairSlot(Ovl11RecordPairSlot *slot); /* both indices = -1 */

#endif
