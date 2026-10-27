#ifndef OVL11_RECORD_PAIR_SLOT_OUTPUT_H
#define OVL11_RECORD_PAIR_SLOT_OUTPUT_H

#include "common.h"

/* Resolve recordOrSlot directly when fromSlot==0, otherwise take its value
 * slot CONTENT and narrow it to a signed halfword before resolving:
 *  ID 29 (hex) -> work +5246/+5248;
 *  ID >=32    -> (ID-32)*F8 record, work +DEC4/+DEC6;
 *  other IDs  -> ID*1D4 record, work +9A0C/+9A0E.
 * Both components are signed halfwords; the comparison pair is the current
 * calendar season/day at work +44BA/+44BC. Source record layouts stay distinct.
 *
 * Export each component sign-extended to the corresponding value slot unless
 * that output index is -1. Indices/record IDs are unchecked. The second write
 * wins if output slots alias. No source record is changed.
 * Returns 1 iff BOTH components equal that current season/day, else 0,
 * including when both exports were suppressed. Requires initialized work/pools.
 * All offsets, strides and ID constants above are hexadecimal.
 */
s32 Ovl11StoreRecordPairToSlots(s16 recordOrSlot, s16 fromSlot,
                               s32 firstSlot, s32 secondSlot);

#endif
