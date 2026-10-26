#ifndef OVL11_CONDITION_CAMERA_JITTER_H
#define OVL11_CONDITION_CAMERA_JITTER_H

#include "common.h"

/* Perturb shared camera-target Z in world-coordinate units by 50-Rand(100),
 * i.e. -49..50, wrapping the stored halfword rather than clamping. Runs only
 * for condition codes 3/4 with (scene mode & 7)==3 and gate 0x08000000 clear.
 * Otherwise neither camera state nor the random stream changes. The area-frame
 * caller passes 1; the argument is unused. Not reentrant; uses shared work RAM. */
void Ovl11JitterCameraTargetZ(s32 unused);

#endif
