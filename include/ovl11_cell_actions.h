#ifndef OVL11_CELL_ACTIONS_H
#define OVL11_CELL_ACTIONS_H

#include "game_types.h"

/* Coordinate services return 1 when applied, 0 when rejected. Layers 0/1
 * select main/alternate grids; other layer indices are invalid for texture
 * dispatch. Cell lookup is checked, but footprint neighbors are NOT checked.
 * Shared RNG, effect pools, counters and texture scratch may be mutated. */

/* Kind 168/169 only; reject main layer in season 3 and unsupported item kinds.
 * Set pending kind to 16A/16B, clear state and flag 8000, dispatch texture 3/4.
 * Kind 33 consumes Rand(25), choosing 179 on zero or 178 otherwise. */
s32 Ovl11TryInstallCellItem(s16 layer, s16 column, s16 row, s16 kind);

/* Promote pending 168->169 or 16A->16B, dispatch texture 2/4. Reject active
 * kind 36 and main layer in season 3. Fourth argument is retained, unused;
 * observed callers supply both 0 and 1. It is not an enable flag. */
s32 Ovl11TryPromoteCellPendingKind(s16 layer, s16 column, s16 row, s16 unused);

/* Category-800 gate: reset to 168/169, clear state/flag 8000, submit a
 * positioned effect and refresh texture/neighborhood. Main season 3 rejects. */
s32 Ovl11TryResetCellAndSpawnEffect(s16 layer, s16 column, s16 row);

/* Resolved-record operation: validate the state's per-kind threshold, then
 * refresh it or restore pending kind and clear state/flags. Return resulting
 * item ID with the original season-2/3 remaps; zero means not resolved.
 * Preserve the canonical record's signed/unsigned kind views. */
u16 Ovl11ResolveCellItemState(Ovl11CellActionEntry *entry);

/* Category-8000 gate: spawn a cell effect and refresh kind 36 or reset other
 * kinds to pending. Kind 36 also updates the shared counter; resulting kind
 * 36/40 submits another positioned effect. */
s32 Ovl11TryApplyCellActionEffect(s16 layer, s16 column, s16 row);

/* Category-2000/4000 gates plus per-kind amount/power thresholds. Inputs are
 * s16 converted to u32 for comparison, not clamped. Kinds select one cell or
 * an unchecked 2x2 footprint. Additional footprint cells ALWAYS address the
 * main grid, even if layer selected an alternate center; caller must arrange
 * valid storage/coordinates. The scored variant also updates a shared counter.
 * These two operations intentionally have different default texture effects. */
s32 Ovl11TryClearCellFootprint(s16 layer, s16 column, s16 row, s16 power, s16 amount);
s32 Ovl11TryClearCellFootprintScored(s16 layer, s16 column, s16 row, s16 power, s16 amount);

#endif
