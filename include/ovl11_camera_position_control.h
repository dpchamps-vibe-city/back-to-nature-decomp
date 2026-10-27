#ifndef OVL11_CAMERA_POSITION_CONTROL_H
#define OVL11_CAMERA_POSITION_CONTROL_H

#include "common.h"

/* ovl_11 must be resident. Coordinates use the shared camera target's integer
 * world units, not pixels or Q12. Y narrows to s16 on entry; X/Z remain s32
 * in the cache and narrow to s16 when published. One shared, non-reentrant
 * move is supported; callers must keep distance arithmetic in s32 range.
 *
 * speedCode -1 writes the target immediately and returns 1, WITHOUT clearing
 * or refreshing an existing move's cache. Otherwise -2/0/1/2/3 select nominal
 * steps 5/24/36/48/72; other codes default to 24. Each call divides each axis
 * delta by floor(max(horizontal distance, abs(Y delta)) / step).
 * A sub-step remainder snaps to the destination but still returns 0; the
 * next equal-target call returns 1 and clears the active flag. No clock is
 * read, so the caller determines the update rate.
 *
 * Every call invalidates selected-record tracking, sets work +C bit 400000,
 * enters position control and clears work +5234 bit 2. Completion leaves
 * these control flags set; releasing them is a separate operation.
 * Original identity: ovl_11:800E60A0. */
s32 Ovl11MoveCameraTarget(s16 speedCode, s32 height, s32 x, s32 z);

/* release == 0 sets work +5234 bit 800000. Nonzero clears that bit and 400,
 * clears work +C bit 400000, and sets/clears work +51FE bit 8 according to
 * whether the signed state at +52C6 is outside/inside {1,6}.
 * keepFlag2 == 0 clears work +5234 bit 2; nonzero leaves it unchanged.
 * Always returns 1. No coordinates or interpolation cache are changed.
 * The last two arguments are unused but retained for the command/caller ABI.
 * Original identity: ovl_11:800E63C8. */
s32 Ovl11SetPositionControlFlags(s16 release, s16 keepFlag2, s32 unused2, s32 unused3);

#endif
