#ifndef POINT_PROJECTION_H
#define POINT_PROJECTION_H

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

/* Project one signed-halfword position using the current GTE rotation,
 * translation and projection settings. On success, optionally store two
 * sign-extended screen coordinates (pixels) in screen[0..1], and return
 * SZ3 >> 2, the depth used by callers to select an ordering-table entry.
 * If GTE FLAG is negative, return zero and leave screen untouched. A zero
 * return is not an unambiguous error indicator: successful depth may be zero.
 * Clobbers the GTE projection/result registers; does not restore GTE state. */
s32 ProjectPointToScreen(s32 *screen, SVECTOR *position);

#endif
