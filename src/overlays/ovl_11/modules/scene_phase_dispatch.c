/*
 * scene_phase_dispatch — reconstructed translation unit.
 *
 * Dispatch the shared scene phase through the eight-entry overlay callback
 * table, passing its base as the callback context.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_scene_phase_dispatch.h"

void Ovl11DispatchScenePhase(void) {
    s32 *callbacks = gOvl11ScenePhaseCallbacks;

    /* Keep the original word-table view: the phase handlers have different
     * source signatures, but this entry always passes the table base. */
    ((void (*)(s32 *))callbacks[gScenePhase])(callbacks);
}
