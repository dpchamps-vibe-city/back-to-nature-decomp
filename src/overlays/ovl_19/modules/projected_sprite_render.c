/* Surface-adjusted sprite projection and depth-bucket submission.
 * The paired paths share the same projection/draw protocol but retain
 * separate screen/vector workspaces in the original overlay data.
 * Interface: ovl19_projected_sprite.h. Grouping: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl19_projected_sprite.h"
#include "ovl19_scene_camera_geometry.h"
#include "sprite_source.h"
#include "sprite_render.h"
#include "point_projection.h"

void func_800248E8(s32 orderingTable, s16 x, s16 y, s16 frame);

void Ovl19DrawProjectedSprite(s32 orderingTable, SpriteSourceData *sprite,
                              s16 *position, s16 screenYOffset) {
    s32 activeOrderingTable;
    s32 depth;
    s32 nearOffset;
    s32 offset;
    s32 spriteBucket;
    s32 companionBucket;

    Ovl19AdjustPositionToSurface(position);
    gOvl19ProjectedSpriteVector[0] = position[0];
    gOvl19ProjectedSpriteVector[1] = position[1];
    gOvl19ProjectedSpriteVector[2] = position[2];
    depth = ProjectPointToScreen(gOvl19ProjectedSpriteScreen, (SVECTOR *) gOvl19ProjectedSpriteVector);
    gOvl19ProjectedSpriteScreen[1] += screenYOffset;
    offset = (depth >> 3) * 4;
    activeOrderingTable = gActiveRenderContext->field_120;
    spriteBucket = orderingTable + offset;
    companionBucket = activeOrderingTable + offset;
    if ((sprite == (SpriteSourceData *) gOvl19ActorSprites) ||
        (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x40)) ||
        (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x80))) {
        DrawSpriteAnimationQuadsAndUpdate(spriteBucket, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
        func_800248E8(companionBucket, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1], 0);
        return;
    }
    /* The +0x120 source bypasses depth rebucketing and the companion. */
    if (sprite == (SpriteSourceData *) (gOvl19ActorSprites + 0x120)) {
        DrawSpriteAnimationQuadsAndUpdate(orderingTable, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
        return;
    }
    if ((position[1] != -0x64) && ((position[2] - position[0]) >= 0)) {
        nearOffset = (depth >> 2) * 4;
        spriteBucket = orderingTable + nearOffset;
        companionBucket = activeOrderingTable + nearOffset;
    }
    DrawSpriteAnimationQuadsAndUpdate(spriteBucket, sprite, (s16) gOvl19ProjectedSpriteScreen[0], (s16) gOvl19ProjectedSpriteScreen[1]);
    /* General sprites keep the companion at unshifted screen Y; the three
     * actor sources above instead apply screenYOffset to both submissions. */
    func_800248E8(companionBucket, (s16) gOvl19ProjectedSpriteScreen[0], (s16) (gOvl19ProjectedSpriteScreen[1] - screenYOffset), 0);
}

void Ovl19DrawProjectedSpriteState(s32 orderingTable, ObjectState *sprite,
                                   s16 *position, s16 animation) {
    s32 depth;
    s32 offset;

    Ovl19AdjustPositionToSurface(position);
    gOvl19ProjectedStateVector[0] = position[0];
    gOvl19ProjectedStateVector[1] = position[1];
    gOvl19ProjectedStateVector[2] = position[2];
    depth = ProjectPointToScreen(gOvl19ProjectedStateScreen, (SVECTOR *) gOvl19ProjectedStateVector);
    SelectSpriteAnimation(sprite, animation & 0xFF);
    offset = (depth >> 3) * 4;
    DrawSpriteAnimationQuadsAndUpdate(orderingTable + offset, (SpriteSourceData *) sprite,
                 (s16) gOvl19ProjectedStateScreen[0], (s16) gOvl19ProjectedStateScreen[1]);
    func_800248E8(gActiveRenderContext->field_120 + offset,
                 (s16) gOvl19ProjectedStateScreen[0], (s16) gOvl19ProjectedStateScreen[1], 0);
}
