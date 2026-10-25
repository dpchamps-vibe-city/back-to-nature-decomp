#ifndef SPRITE_GRID_H
#define SPRITE_GRID_H

#include "common.h"
#include "game_types.h"

/* Draw a scrolling 15-column by 11-row sprite grid into the ordering table
 * whose ADDRESS is passed as a signed word. Cells are 24 pixels apart;
 * animation 29 uses four frame indices from gSpriteGridFrameIndices.
 * The ordinary entrypoint reserves/advances the active packet-work cursor;
 * InContext instead advances the cursor at active render context +0x118.
 * Both require initialized render state and loaded gSharedSpriteData. */
void DrawScrollingSpriteGrid(s32 orderingTable);
void DrawScrollingSpriteGridInContext(s32 orderingTable);

/* Initialize gSharedSpriteSource, increment the wrapping 32-bit grid tick,
 * and call visitor 165 times with that source, a signed frame index and x/y
 * pixel coordinates. The diagonal offset is (tick / 4) modulo 24; the frame
 * pattern carries across row boundaries. No clipping is performed here.
 * The shared source is reinitialized on each grid call, so this is not reentrant
 * with other users of gSharedSpriteSource. The visitor must not replace it. */
void VisitScrollingSpriteGrid(s32 orderingTable, SpriteGridCallback visitor);

#endif
