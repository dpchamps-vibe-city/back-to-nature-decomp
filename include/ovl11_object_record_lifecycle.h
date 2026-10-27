#ifndef OVL11_OBJECT_RECORD_LIFECYCLE_H
#define OVL11_OBJECT_RECORD_LIFECYCLE_H

#include "common.h"
#include "game_types.h"

/* First zero-tag slot in the six-record gStateObjectPool, or NULL when full.
 * The ID is narrowed to s16 for the existing initializer; it must name a valid
 * object definition. Copy the shared eight-byte work snapshot to +1A, save
 * the shared selector at +30, set +AC to 255, and enter state 4 for tag 0x157 or
 * state 0x12 for tag 0x15B. Other IDs keep the initializer's state. Storage belongs
 * to the resident pool and may be reset/reused by other record services. */
Ovl11ObjectInitRecord *Ovl11AllocateStateObjectRecord(s32 id);

/* Raw address-as-s32 ABI. Add the three signed words at +48/+4C/+50 to
 * position +38/+3C/+40, then zero those deltas. No coordinate scaling or timer
 * step; the caller supplies a writable initialized record. */
void Ovl11AdvanceStateObjectPosition(s32 recordAddress);

/* Resolve animation/frame from record +24/+22. Animation changes only when
 * different; frame zero means keep the current frame, not select frame zero.
 * Clear the shared animation-update latch, submit through
 * Ovl11DrawObjectWithMarker (including its animation/gate behavior), then set
 * sprite flag 0x100 when +24 is zero. Same graphics/GTE/packet prerequisites and
 * shared-state non-reentrancy as ovl11_object_render.h; no GPU presentation. */
void Ovl11DrawStateObjectRecord(u8 *record);

#endif
