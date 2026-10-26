#ifndef MODE6_TEXTURE_REFRESH_H
#define MODE6_TEXTURE_REFRESH_H

#include "common.h"

/* Farm overlay, active area mode 6. Column/row are grid indices (not world
 * coordinates); callers must supply valid cells and texture-table indices.
 * The grid has a 45-pointer row stride even for this 7x7 area.
 *
 * Set patches UV4, tpage and CLUT on the two supported header classes with
 * bit 3 set. Row zero reverses the UV order; indices >=5 use tpage 0x1D,
 * others use 9. Returns 0 both after a patch and outside mode 6, or 1 for an
 * unsupported entry. It is not a success Boolean. No null/bounds checks.
 * Refresh derives indices from the 7x7 map records and ignores Set's result.
 */
s32 Ovl11SetMode6CellTexture(s16 column, s16 row, s16 textureIndex);
void Ovl11RefreshMode6CellTextures(void);

#endif
