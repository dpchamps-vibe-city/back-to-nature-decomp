#ifndef OVL11_CLOCK_SCENE_COMPLETION_H
#define OVL11_CLOCK_SCENE_COMPLETION_H

#include "common.h"

/* ovl_11:800DE46C. Per-frame consumer of flag 0x49 (any nonzero value).
 * Snapshot resident +44CA to primary-object +2C, set work+4450 bit 4,
 * request the minutes to the NEXT 06:00 plus 360, then clear flag 0x49.
 * Replaces an earlier advance request; does not itself run the clock.
 * All clock quantities are minutes; the 06:00 boundary follows clock_calendar. */
void Ovl11ConsumeDeferredClockAdvance(void);

/* ovl_11:800DE4C0. Shared completion service for clock callbacks/advances.
 * Refresh hour tint. Flag 0x48==1 takes precedence over 0x9D==1 and requests
 * scene 7 or 16 respectively, clearing that flag and flag 0x47.
 * Otherwise clear 0x47 and, in far-work mode 2, process pending condition work
 * and drain active record service until runnable or flag 2 clears (no timeout).
 * Except in modes 0x18/0x19, start a 45-step screen ramp and master-volume
 * fade-in with divisor 10 (integer ramps do not guarantee exact duration). With work bit 4 clear and flag 2 zero, replace the held
 * item with code 0x11B..0x11E; hours >=7 also set flag 0xAD. Finally clear
 * resident+0C bit 0x800000. Requires resident ovl_11, initialized scene,
 * render/audio and record services; not reentrant. No return value. */
void Ovl11CompleteClockSceneEvent(void);

#endif
