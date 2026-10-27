#ifndef OVL11_OBJECT_POSITION_SLOT_TRANSFER_H
#define OVL11_OBJECT_POSITION_SLOT_TRANSFER_H

#include "common.h"

/* These commands use height, X, Z slot order, not memory's X, height, Z.
 * Slots are the shared twenty-word script array; positions retain the source
 * record system's coordinate units. No scaling or rounding is performed. */

/* Read height as the slot's signed LOW HALFWORD, X/Z as full s32 words,
 * forward to the record-position command and return its result. There are
 * no slot guards here, including no -1 sentinel; all indices must be valid. */
s32 Ovl11ApplySlotPositionTriple(s16 recordId, s16 heightSlot, s32 xSlot, s32 zSlot);

/* ID 41 selects gObjectWorkPosition, IDs 1..36 select coordinate triples
 * at gObjectPositionRecordCoordinates + ID*0x1D4 bytes. Other IDs return 0
 * without writing. Each -1 slot independently suppresses that component;
 * other indices are unchecked. Copy full words; return 1 for a valid record. */
s32 Ovl11SaveRecordPositionToSlots(s16 recordId, s16 heightSlot, s32 xSlot, s32 zSlot);

/* Save three LIVE signed-halfword coordinates, sign-extended into slots.
 * Nonzero consecutive replaces X/Z slots with heightSlot+1/+2 and returns
 * 0 immediately for heightSlot == -1. Otherwise components are independent.
 * Each write skips -1 and indices >=20, but there is NO general lower-bound
 * check: callers must not supply other negative indices. Returns 1 even
 * when every component is skipped. Does not mutate the live position. */
s32 Ovl11SaveLivePositionToSlots(s16 consecutive, s16 heightSlot, s32 xSlot, s32 zSlot);

#endif
