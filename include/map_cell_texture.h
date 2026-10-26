#ifndef MAP_CELL_TEXTURE_H
#define MAP_CELL_TEXTURE_H

#include "common.h"

/* Farm-overlay mode-1 map grid, 25 rows by 45 columns.
 *
 * Build probes the scene at 400-coordinate-unit steps (x starts -4200,
 * z starts 1500), installing hit pointers whose header has bit 3 set.
 * A zero low halfword of resumeWord restarts the retained row scan; any
 * other value resumes it. Returns 1. Collision geometry, far-work-area
 * storage and the hit pointer must already be valid; no miss/null guard.
 *
 * Apply derives texture indices from all 25x45 map records. Set accepts
 * column/row/texture indices, patches UV4/CLUT/tpage in the two supported
 * header classes, and applies the existing field-44BA overrides. Returns 0
 * both after a patch and outside mode 1, or 1 for an unsupported entry.
 * Indices and entry pointers are unchecked. The table has 30 UV records;
 * the caller must keep its encoded variants in that domain.
 * These operations share retained state and are not reentrant.
 */
s32 Ovl11SetMapCellTexture(s16 column, s16 row, s16 textureIndex);
void Ovl11ApplyMapTileTextures(void);
s32 Ovl11BuildMapGrid(s32 resumeWord);

#endif
