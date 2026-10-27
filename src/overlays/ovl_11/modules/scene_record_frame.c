/*
 * scene_record_frame — reconstructed translation unit.
 *
 * Service the gated scene-record frame in refresh, draw, then sprite-offset
 * decay order.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_scene_record_frame.h"
#include "ovl11_projected_record_sprites.h"
#include "ovl11_record_pool_refresh.h"

void ovl_11_func_800F4A58(void);

void Ovl11ServiceSceneRecordFrame(void) {
    if (!(gSharedWorkFlags & 0x4000)) {
        Ovl11RefreshDispatchRecordPools();
        Ovl11DrawSceneRecordSprites();
        ovl_11_func_800F4A58();
    }
}
