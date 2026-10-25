#ifndef OVL11_POSITION_TRANSITION_H
#define OVL11_POSITION_TRANSITION_H

#include "common.h"
#include "game_types.h"

/* ovl_11 must be resident. One shared transition; calls are not reentrant.
 * Coordinates retain the object's integer units (no scale conversion).
 *
 * Begin captures the object's +100/+104/+108 position and chooses an X/Z
 * destination from script codes 50..5B (hex), with offsets -550/+440.
 * It installs object mode 4A, sets object/work flags, and resets the step.
 * It does not advance the position. A second Begin replaces the first path.
 *
 * Step samples indices 0..17 inclusive, publishing truncated halfword XYZ
 * to the shared work position and full-word X/Z to the object. Object Y
 * accumulates a separate signed displacement, capped at start-Y minus 40;
 * the published Y instead clamps the linear sample to that lower bound.
 * Returns 0 while advancing, 1 at step 17, clears work bit 00400000 and
 * fixes object Y at start-Y minus 40. Completion does not reset the step.
 * Begin must precede Step; caller controls when/how often Step runs. */

/* Original identities: ovl_11:800CE210 and ovl_11:800CE37C. */
void Ovl11BeginPositionTransition(Ovl11C7010Object *object);
s32 Ovl11StepPositionTransition(Ovl11PositionTransitionObject *object);

#endif
