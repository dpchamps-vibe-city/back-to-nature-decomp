/*
 * pending_stage_request — reconstructed translation unit.
 *
 * Install the single pending signed-halfword mode consumed by the stage
 * dispatcher.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"

#include "ovl11_pending_stage_dispatch.h"

void Ovl11SetPendingStageMode(s16 mode) {
    gOvl11PendingStageMode = mode;
}
