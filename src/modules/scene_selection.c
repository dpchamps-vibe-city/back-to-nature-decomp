/*
 * Scene-selection handshake between overlay callbacks and the EXE dispatcher.
 * Requesting a change records the two dispatch IDs and ends the active update
 * loop; the entry loop performs the next scene's setup after that loop returns.
 *
 * This is a reconstructed slice of the larger boot/main-loop family, not a
 * proven original file boundary. See notes/file-groupings.md.
 */

#include "common.h"
#include "scene_selection.h"

/* Shared with boot/dispatch and the update loop, not private module storage.
 * Tentative definitions preserve the original GP-relative accesses. */
s32 gPreviousSceneId;
s32 gCurrentSceneId;
s32 gSceneUpdateLoopActive;

void RequestSceneChange(s32 sceneId) {
    s32 previousSceneId;

    /* Snapshot before replacing the ID, even for a repeated/same-ID request. */
    previousSceneId = gCurrentSceneId;
    gSceneUpdateLoopActive = 0;
    gCurrentSceneId = sceneId;
    gPreviousSceneId = previousSceneId;
}

s32 GetPreviousSceneId(void) {
    return gPreviousSceneId;
}

s32 GetCurrentSceneId(void) {
    return gCurrentSceneId;
}
