#ifndef OVL11_GRID_CELL_QUERIES_H
#define OVL11_GRID_CELL_QUERIES_H

#include "common.h"
#include "game_types.h"

/* Overlay 11 grid queries. The main grid is 45 columns by 25 rows; the
 * alternate grid is 7 by 7. Grid/item pools and world-to-cell transforms
 * must already be initialized. Positions use the engine's world units.
 */

/* Probe one and two direction-table offsets from XYZ. Only area selector
 * 1 (main) or 6 (alternate) is supported. Return 1 when the near cell has
 * property 1 and the far cell is empty/property 0 or outside the grid.
 * Main-grid column 44 with direction 3 is rejected. direction is an
 * unchecked index into the shared signed X/Z offset-pair table.
 * Original identity: ovl_11:800BF4B4. */
s32 Ovl11CheckDirectionalCellTransition(s32 *position, s32 direction);

/* Read only query->position and query->object's signed region halfword.
 * Return 1 only for region 1, a valid main-grid cell, and tag 0x3B..0x3D,
 * 0x3F or 0x16C..0x174. No hit/flags outputs are written.
 * Original identity: ovl_11:800BF720. */
s32 Ovl11IsSpecialCollisionCell(Ovl11CollisionQuery *query);

/* Submit action 0 with enabled=1 through the existing grid-cell service
 * for all 1125 main-grid coordinates, row-major. That service may update
 * cell state and dispatch callbacks; this is not a read-only query.
 * Original identity: ovl_11:800BF844. */
void Ovl11ProcessMainGridCells(void);

#endif
