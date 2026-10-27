/*
 * scene_draw — reconstructed translation unit.
 *
 * Scene drawing traversal: visits six record sprites, their three active slot
 * sprites, and the primary indexed sprite; a separate entry draws the
 * centered frame.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl21_scene_draw.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl21_scene_sprite_draw.h"

void Ovl21DrawRecordSprites(void);

void Ovl21DrawActiveSlotSprites(void);

void Ovl21DrawPrimarySpriteAtSelectedOffset(void);

void Ovl21DrawSceneSprites(void) {
    Ovl21DrawRecordSprites();
    Ovl21DrawActiveSlotSprites();
    Ovl21DrawPrimarySpriteAtSelectedOffset();
}

void Ovl21DrawSceneFrame(void) {
    Ovl21DrawCenteredFrameRects();
}

void Ovl21DrawRecordSprites(void) {
    u16 markerPosition[3];
    u8 *objectBase;
    UnkStruct800C0448 *record;
    s32 remaining;
    s32 *object;
    SpriteSourceData *markerSprite;

    objectBase = gOvl21RecordObjectBase;
    markerSprite = (SpriteSourceData *)(objectBase + 0x6D4);
    record = (UnkStruct800C0448 *)(objectBase - 0x10);
    object = (s32 *)objectBase;
    remaining = 5;
    do {
        Ovl21DrawRecordSprite(object);
        if (record->unk18 == 2 && (((SpriteSourceData *)record->unk30)->field_2 & 0x100)) {
            markerPosition[0] = record->unk1A;
            markerPosition[1] = record->unk1C - 0x15E;
            markerPosition[2] = record->unk1E;
            SelectSpriteAnimation((ObjectState *)markerSprite, 0);
            Ovl21SubmitProjectedSprite(markerSprite, markerPosition);
        }
        record = (UnkStruct800C0448 *)((u8 *)record + 0x108);
        remaining -= 1;
        object = (s32 *)((u8 *)object + 0x108);
    } while (remaining >= 0);
}

void Ovl21DrawActiveSlotSprites(void) {
    u8 *sceneBase;
    s32 *slots;
    s32 *slot;
    s32 *active;
    s32 recordIndex;
    s32 nextRecord;
    s32 slotsRemaining;
    s32 recordOffset;

    recordIndex = 0;
    sceneBase = (u8 *)gOvl21SceneControl;
    slots = (s32 *)(sceneBase + 0x34);
    while (recordIndex < 6) {
        nextRecord = recordIndex + 1;
        recordOffset = ((recordIndex << 5) + recordIndex) << 3;
        slot = (s32 *)(recordOffset + (s32)slots);
        active = slot;
        for (slotsRemaining = 2; slotsRemaining >= 0; slotsRemaining--) {
            if (*active != 0) {
                Ovl21DrawSlotSprite(slot);
            }
            slot = (s32 *)((u8 *)slot + 0x4C);
            active = (s32 *)((u8 *)active + 0x4C);
        }
        recordIndex = nextRecord;
    }
}

void Ovl21DrawPrimarySpriteAtSelectedOffset(void) {
    Ovl21DrawPrimarySceneSprite(gOvl21PrimarySpriteOffsetIndex);
}
