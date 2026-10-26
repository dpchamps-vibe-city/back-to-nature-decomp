#ifndef OVL11_FLAGGED_VOLUME_CODE_H
#define OVL11_FLAGGED_VOLUME_CODE_H

#include "common.h"
#include "game_types.h"

/* Original ovl_11:800C79F8. Query the current interaction-volume list
 * (far work +0x25388) at position. Each s32 coordinate is narrowed to its low
 * signed halfword; there is no Q12 shift, clamp or position mutation.
 * Return 0x26, 0x2A, 0x2B or 0x2C for those volume codes, otherwise zero.
 * gOvl11LastFlaggedVolumeAddress retains the found address even when its
 * code is unrecognized; zero means no hit. The list must be initialized.
 * The volume records themselves are not modified. */
s32 Ovl11QueryFlaggedVolumeCode(Vec3 *position);

#endif
