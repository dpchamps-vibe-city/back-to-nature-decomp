/*
 * scene_phase — reconstructed translation unit.
 *
 * Reset the shared scene dispatch phase and step when the main loop enters a
 * new overlay. This does not initialize overlay-private state.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "scene_phase.h"

void ResetSceneDispatchState(void) {
    s32 *state;

    state = &D_8006C838;
    /* Retain the common state-base view used by the overlay dispatchers. */
    *(s32 *)((char *)state + 0x4488) = 0; /* gScenePhase */
    *(s32 *)((char *)state + 0x448C) = 0; /* gSceneSubphase */
}
