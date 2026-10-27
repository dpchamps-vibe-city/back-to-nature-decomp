#ifndef OVL11_RECORD_PAIR_CLAIM_H
#define OVL11_RECORD_PAIR_CLAIM_H

#include "common.h"

/* Claim B4 pairIndex 0/1 (shared slot pairIndex+2), requiring its expansion
 * slot to be enabled. Returns 0 on success, 0x1E6 if occupied/disabled, or
 * 0x1A1 if no enabled free B4 record remains. Failed claims change nothing.
 * Success reserves the zero-type record with flag 0x02000000 and writes its
 * index plus linkedRecordIndex to the two signed halfwords at work+0x99C8.
 * Indices narrow to those halfwords; no bounds/range checks or record
 * initialization are added. The linked index belongs to the caller's protocol.
 */
s32 Ovl11ClaimRecordPairSlot(s32 pairIndex, s32 linkedRecordIndex);

#endif
