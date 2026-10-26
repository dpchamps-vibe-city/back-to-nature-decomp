#ifndef OVL11_OBJECT_STATE_DISPATCH_H
#define OVL11_OBJECT_STATE_DISPATCH_H

#include "common.h"

/* Operate on the existing object-record ABI (not a u16 array of known size).
 * ID +0, state +26, phase +28, program cursor +2A, timer +2C, flags +34 and
 * vector/source +48 must be valid. State indices are unchecked: 0..20.
 * Records and callback tables must remain live across these calls. */

/* Refresh the record's sprite source; ID zero before/after refresh returns
 * -1. Otherwise invoke the nullable current-state frame callback. When flag
 * 0x800 is clear and signed timer >=300, choose a new state and reset timer;
 * then increment timer. Returns 0, ignoring the frame callback's result. */
s32 Ovl11UpdateObjectState(u16 *record);

/* Run the nullable requested-state entry gate under the existing same-state
 * and 0x800 rules (state 6 has a special bypass). A gate result of -1 vetoes
 * the transition. If a frame callback exists and no veto occurred, store the
 * state, zero +28/+2A/+2C and initialize +48 with D0408 mode 4. Does NOT run
 * the frame callback. Return the gate result (initially 0), even when no
 * transition occurs. No range validation or allocation is performed. */
s32 Ovl11EnterObjectState(u16 *record, s32 state);

#endif
