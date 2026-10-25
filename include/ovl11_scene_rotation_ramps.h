#ifndef OVL11_SCENE_ROTATION_RAMPS_H
#define OVL11_SCENE_ROTATION_RAMPS_H

#include "common.h"

/* ovl_11 must be resident. Two independent ramps modify selected records'
 * base Y/Z angles (GTE units: 4096 per revolution). The renderer adds other
 * angle offsets later; these calls do not draw or change those offsets.
 * Shared record pointers and phase words make this interface non-reentrant.
 *
 * Init selects the FIRST of fifteen records with flag 0x40 (Z) or 0x20 (Y),
 * zeros ALL THREE base angles, and resets that ramp to idle. With no match,
 * the saved pointer is null and Step is a no-op. The record pool must remain
 * valid until the next Init; no allocation or ownership transfer occurs.
 *
 * Step samples game flag 0x71 (Z) or 0x70 (Y) while idle, then advances
 * 32 units per call toward Z=640 or Y=-448 and holds there. Only while held
 * does it test for flag release; it then returns to zero at the same rate.
 * Trigger/release calls change phase without changing the angle. A ramp
 * completes its outward/return leg even if the flag changes during that leg.
 * Held queries return 1 exactly in phase 2, otherwise 0; no state change. */

/* Original identities: ovl_11:800DD8AC, 800DD904, 800DD9F0. */
void Ovl11InitSceneZRotationRamp(void);
void Ovl11StepSceneZRotationRamp(void);
s32 Ovl11IsSceneZRotationRampHeld(void);

/* Original identities: ovl_11:800DDA08, 800DDA60, 800DDB4C. */
void Ovl11InitSceneYRotationRamp(void);
void Ovl11StepSceneYRotationRamp(void);
s32 Ovl11IsSceneYRotationRampHeld(void);

#endif
