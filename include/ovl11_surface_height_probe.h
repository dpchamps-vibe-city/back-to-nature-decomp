#ifndef OVL11_SURFACE_HEIGHT_PROBE_H
#define OVL11_SURFACE_HEIGHT_PROBE_H
#include "common.h"
#include "game_types.h"

/* Try the five signed tolerances in gOvl11PositionHeightTolerances in order.
 * On the first surface hit, replace position->y and return 0; on failure,
 * leave the caller's position unchanged and return 1. X/Z never change.
 * Coordinates use the surface resolver's native fixed-point units. Requires
 * loaded area geometry. Always resets the SHARED height tolerance to 200,
 * not its previous value; this service is not reentrant. */
s32 Ovl11ProbePositionHeight(Vec3 *position);
#endif
