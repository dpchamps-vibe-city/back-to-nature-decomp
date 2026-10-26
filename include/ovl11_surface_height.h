#ifndef OVL11_SURFACE_HEIGHT_H
#define OVL11_SURFACE_HEIGHT_H

#include "common.h"

/* position points to three signed world-coordinate words (X,Y,Z).
 * Return 0 after updating only Y, or 1 without changing the position.
 * Modes 0xC080/0xE080 may resolve to Y=0 through collision queries; in the
 * first 0xC080 branch a negative input Y fails instead. Otherwise mode is
 * installed in the shared polygon-query state and Y comes from its hit plane.
 * Requires initialized scene geometry/collision work. Consumes the common
 * collision-query count on each special-mode probe and replaces last-hit
 * geometry state on the polygon path; not reentrant. No bounds/null checks. */
s32 Ovl11ResolveSurfaceHeight(s32 *position, s32 mode);

#endif
