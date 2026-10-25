#ifndef OVL11_CELL_SPRITE_DISPATCH_H
#define OVL11_CELL_SPRITE_DISPATCH_H

#include "common.h"

/* Draw the visible part of the 45x25 cell map, unless work flag 0x200 is set.
 * View-center coordinates define a +/-3600 scene-unit window; cells are 400
 * units apart. Mode 0/1/2 (otherwise fallback) selects the private kind/value
 * resolver, which is stored in gOvl11CellSpriteResolver. Projection failures
 * and unsupported cell kinds draw nothing. Uses the active context's secondary
 * OT and work-area sprite sources; y offsets are projected screen pixels. */
void Ovl11DrawVisibleCellSprites(void);

/* Draw the separate 7x7 cell map through the same conditional-output protocol,
 * but a different explicit kind-to-texture mapping and engine sprite bank.
 * Cell spacing is 400 scene units. No map, sprite state or mode is updated. */
void Ovl11DrawFixedCellSpriteGrid(void);

#endif
