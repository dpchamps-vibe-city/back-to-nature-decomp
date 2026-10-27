#ifndef OVL11_OBJECT_AREA_SPRITE_RESET_H
#define OVL11_OBJECT_AREA_SPRITE_RESET_H

#include "common.h"
#include "game_types.h"

/* Clear only bit 0x1000 in object's +0x6C flag word, then reinitialize the
 * SHARED area sprite source from the installed staging header and reset the
 * GLOBAL primary-object request. The supplied object is not initialized or
 * freed. Requires valid area resource/staging state; no pointer checks. */
void Ovl11ResetObjectAreaSpriteSource(Recon_ovl_11_func_800CD08C_A0View *object);

#endif
