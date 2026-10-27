#ifndef OVL11_SLOT_SPRITE_SOURCE_REFRESH_H
#define OVL11_SLOT_SPRITE_SOURCE_REFRESH_H
#include "common.h"

/* Clear the shared slot-sprite enable latch. Only season 3 enables it again,
 * and only if resident sprite 0x187 exists, in which case initialize the shared
 * sprite source from its header. Missing/ineligible data leaves the old source
 * contents intact but disabled. Requires the resident slot bank to be loaded;
 * no allocation, upload or draw occurs here. */
void Ovl11RefreshSharedSlotSpriteSource(void);
#endif
