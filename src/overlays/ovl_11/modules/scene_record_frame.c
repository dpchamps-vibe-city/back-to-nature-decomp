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
#include "ovl11_record_sprite_offset_decay.h"

void Ovl11ServiceSceneRecordFrame(void) {
    if (!(gSharedWorkFlags & 0x4000)) {
        Ovl11RefreshDispatchRecordPools();
        Ovl11DrawSceneRecordSprites();
        Ovl11DecayCurrentRecordSpriteOffset();
    }
}
