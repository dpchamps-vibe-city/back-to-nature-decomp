/*
 * sprite_pause — reconstructed translation unit.
 *
 * Scene sprite pause service: the two public wrappers select set/clear flag 4
 * through their exclusive bulk helper, visiting sixteen primary sources and
 * eighteen record-slot sources.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl21_sprite_pause.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

/* Kept externally linked; co-location alone does not prove static linkage. */
void Ovl21SetSceneSpritePauseMode(s32 paused);

void Ovl21PauseSceneSprites(void) {
    Ovl21SetSceneSpritePauseMode(1);
}

void Ovl21ResumeSceneSprites(void) {
    Ovl21SetSceneSpritePauseMode(0);
}

void Ovl21SetSceneSpritePauseMode(s32 paused) {
    s32 index;
    s32 slotIndex;
    struct SpriteSourceData *sprite;
    u16 *record;
    void (*updateFlags)(u16 *, s32);

    if (paused != 0) {
        updateFlags = AddSpriteSourceFlags;
    } else {
        updateFlags = ClearSpriteSourceFlags;
    }
    updateFlags(&gOvl21PrimarySceneSprite.field_0, 4);
    updateFlags(&(&gOvl21PrimarySceneSprite)[1].field_0, 4);
    updateFlags(&(&gOvl21PrimarySceneSprite)[2].field_0, 4);
    sprite = &gOvl21PrimarySceneSprite + 3;
    for (index = 0; index < 13; index++) {
        updateFlags(&sprite->field_0, 4);
        sprite++;
    }
    for (index = 0; index < 6; index++) {
        record = (u16 *) (gOvl21RecordObjectBase + index * 0x108);
        for (slotIndex = 0; slotIndex < 3; slotIndex++) {
            updateFlags((u16 *) ((u8 *) record + slotIndex * 0x4C + 0x40), 4);
        }
    }
}
