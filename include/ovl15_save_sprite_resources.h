#ifndef OVL15_SAVE_SPRITE_RESOURCES_H
#define OVL15_SAVE_SPRITE_RESOURCES_H

#include "common.h"

/* Blocking save-menu resource setup. Always reads 0x900 bytes of card-icon
 * data from archive 0x10 into the buffer installed by overlay initialization.
 * A nonzero loadSprites also reads archive 0x0E, uploads its images to VRAM,
 * copies 0x2D74 bytes of sprite records and initializes five source objects.
 *
 * Requires the CD archive index, active render context and destination buffers
 * to be initialized. Clears the active 0x800-entry ordering table before each
 * read and polls until the CD loader returns a non-NULL buffer; retries are
 * internal to that loader and there is no timeout. Shared transfer/sprite storage makes
 * this non-reentrant. The transfer workspace is also used by card I/O. */
void Ovl15LoadMenuResources(s16 loadSprites);

#endif
