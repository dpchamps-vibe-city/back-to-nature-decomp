#ifndef OVL21_ENTITY_RECORD_VIEWS_H
#define OVL21_ENTITY_RECORD_VIEWS_H

#include "common.h"

/* Resident ovl_21; entity IDs index the persistent work area unchecked with
 * stride 0x1D4. The values' gameplay units are not established. IDs 7/9/20
 * use the u16 at work+8000+19EC; all others use s16 at work+8000+19EA. */

/* ID 0 is a no-op. Special IDs add 500, replacing results >0xFE0A with
 * 0xFFFF. Others add 20, replacing results >=235 with 255. */
void Ovl21RaiseEntityRangeValue(s16 entityId);

/* Special IDs return SquareRoot0(u16 value), narrowed to s16; otherwise
 * return the raw signed halfword. Unlike the writer, ID 0 is not skipped. */
s16 Ovl21ReadEntityRangeValue(s16 entityId);

/* Signed class constant: IDs 16/34 -> 360, 7/9/20/22 -> 420, else 480
 * in the original data. Constants remain mutable storage, not C literals. */
s32 Ovl21GetEntityRangeConstant(s16 entityId);

/* Select one of 13 0x30-byte sprite objects: 12 mapped IDs and a default.
 * Unmapped IDs (including 0) share the default object; this is not allocation.
 * The returned byte pointer preserves the existing opaque storage interface. */
u8 *Ovl21GetEntityObjectRecord(s16 entityId);

#endif
