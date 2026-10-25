/*
 * Software enable gate shared by both pads' buffered actuator output.
 * The paired accessors expose the same word consumed by request scheduling
 * and the pad output service. This narrow slice of the pad family is a
 * reconstructed interface, not a proven original translation-unit boundary.
 * See pad_actuator_gate.h for deferred effects and notes/file-groupings.md
 * for membership evidence.
 */

#include "common.h"
#include "pad_actuator_gate.h"

/* Retain GP-relative access to the existing word. Startup and the output
 * service also define it tentatively; this slice does not make it private. */
s32 gPadActuatorsEnabled;

void SetPadActuatorsEnabled(s32 enabled) {
    gPadActuatorsEnabled = enabled;
}

s32 GetPadActuatorsEnabled(void) {
    return gPadActuatorsEnabled;
}
