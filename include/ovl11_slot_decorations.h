#ifndef OVL11_SLOT_DECORATIONS_H
#define OVL11_SLOT_DECORATIONS_H

#include "common.h"

/* All coordinates are signed pixel positions relative to a menu cell;
 * orderingTable is its packet-link address. Requires the resident shared
 * sprite data. Calls overwrite its current animation/frame; not reentrant.
 * Each routine submits drawing only, without changing selection or items. */

/* Cursor sprite at cell origin + (12,24), via the engine's cursor service. */
void Ovl11DrawSlotCursor(s32 orderingTable, s16 x, s16 y);
/* Animation B/frame 0 as unscaled textured quads. */
void Ovl11DrawSelectedSlotMarker(s32 orderingTable, s16 x, s16 y);
/* Animation B/frame 2 as rectangles; also used for a missing/zero item ID. */
void Ovl11DrawEmptySlot(s32 orderingTable, s16 x, s16 y);
/* Animation A: frame A at (+2,+22), optional tens at (+12,+22), units at
 * (+22,+22). Intended for 0..99; no range check or clamp is performed.
 * Division/remainder are signed and frame indices are narrowed to bytes. */
void Ovl11DrawSlotQuantity(s32 orderingTable, s16 x, s16 y, s32 quantity);

#endif
