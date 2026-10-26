#ifndef AREA_GRID_TRIGGERS_H
#define AREA_GRID_TRIGGERS_H

#include "common.h"
#include "game_types.h"

/* Farm-overlay area-cell lifecycle (area IDs 0x37/0x43).
 * State contains ten 10x10 byte planes: 0 unclassified, 1 ordinary, 2 trigger.
 * Reset clears planes, per-plane found bytes, active plane and trigger flags;
 * it retains probability and the last/centre coordinate pairs. Build starts
 * the probability at 10, marks plane 9 found and probes a 10x10 portion of the
 * shared 45-stride pointer grid at 400-world-unit steps. Zero low halfword of
 * resumeWord restarts its retained scan; nonzero resumes. It returns 1.
 *
 * Check tests previous/current-plane contact for the supplied object's stored
 * position and latches a trigger (return 1), else returns 0. Classify examines
 * an offset cell, excludes borders/centre and prior-plane triggers, then rolls
 * trigger/reward state. It returns 1 only when it gives a nonzero reward, not
 * when it installs a trigger. Update runs the latched message/choice protocol,
 * advances or resets planes and changes scene/game flags. Restore consumes the
 * saved-cell/game-flag handoff and restores position and centre state.
 *
 * Position maps x/z to column/row via 400-unit signed division: return 0 for
 * an accepted header, 1 for missing header bit 3, 2 below bounds, 3 above.
 * Outputs are u16 storage for signed calculations. A 16-byte position record
 * is read. Geometry, hit pointers, render context and shared messages/fades
 * must be initialized. Grid indices/planes and pointers are not validated;
 * these stateful operations are non-reentrant and do not impose new clamps.
 */
void Ovl11ResetAreaGridState(void);
s32 Ovl11CheckAreaGridContact(Ovl11C7010Object *object);
s32 Ovl11BuildAreaGrid(s32 resumeWord);
s32 Ovl11ClassifyAreaGridCell(Ovl11CE034Object *object);
s32 Ovl11AreaGridCellFromPosition(s32 *position, u16 *column, u16 *row);
void Ovl11UpdateAreaGridTrigger(void);
void Ovl11RestoreAreaGridPosition(void);

#endif
