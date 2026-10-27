#ifndef OVL11_CONDITION_RECORD_FIELDS_H
#define OVL11_CONDITION_RECORD_FIELDS_H

#include "common.h"

/* Decode six 5-bit fields, highest first (bits 29..25 through 4..0), into
 * six writable halfwords. For field value 31, substitute 1 in field 0,
 * 0x7FFF in field 1, and 31 elsewhere. Then subtract one from fields 0..2:
 * an encoded zero there wraps to 0xFFFF, not a clamped zero.
 * The evaluator uses bounds[0..1] as a date range, [2..3] as a date window
 * and [4..5] as a time window. No shared state or table storage is changed.
 */
void Ovl11DecodeConditionBounds(u32 packed, u16 *bounds);

/* Decode three 10-bit fields, LOWEST first, into three writable signed
 * halfwords (values 0..1023). Bits 30..31 are ignored. The caller treats
 * 1023 as no requirement, otherwise tests GetGameFlag(id+0xFB).
 * selector is retained in the s32 ABI; both branches decode identically.
 */
void Ovl11DecodeConditionFlagIds(u32 packed, s16 *ids, s32 selector);

/* Map selector's low 16 bits: 0..4 -> {3,7,9,14,20}; all others -> 3.
 * The condition evaluator uses this as an index into 0x1D4-stride records.
 * This function reads no record and applies no three-bit mask itself.
 */
s32 Ovl11GetConditionRecordSlot(s32 selector);

#endif
