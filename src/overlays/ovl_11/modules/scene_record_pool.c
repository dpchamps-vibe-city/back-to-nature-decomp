/*
 * scene_record_pool — reconstructed translation unit.
 *
 * Lifecycle of fifteen 0x30-byte projected scene records: clear, find a free
 * marker, install scene-map transforms and optional vertex blends, then reset
 * dependent animation controls.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "projected_primitives.h"
#include "ovl11_scene_record_visibility.h"
#include "ovl11_scene_rotation_ramps.h"
#include "ovl11_scene_animation_pair.h"
#include "ovl11_scene_animation_cycle.h"
#include "ovl11_vertex_blend.h"
#include "ovl11_scene_record_pool.h"

void Ovl11ClearSceneRecord(Ovl11SceneRecordResetView *record);

void Ovl11ClearSceneRecords(void) {
    u8 *record;
    s32 remaining;

    record = gOvl11SceneRecords;
    for (remaining = 0xE; remaining >= 0; remaining--) {
        Ovl11ClearSceneRecord((Ovl11SceneRecordResetView *)record);
        record += 0x30;
    }
}

Ovl11SceneRecordSlot *Ovl11FindFreeSceneRecord(void) {
    s32 index;
    Ovl11SceneRecordSlot *record;

    record = (Ovl11SceneRecordSlot *)gOvl11SceneRecords;
    for (index = 0; index < 0xF; index++, record++) {
        if (record->flags & 0x8000) {
            return record;
        }
    }
    return 0;
}

/* Delta angles at +0x18 and the blend pointer are intentionally not reset. */
void Ovl11ClearSceneRecord(Ovl11SceneRecordResetView *record) {
    record->positionX = 0;
    record->positionY = 0;
    record->positionZ = 0;
    record->baseAngleX = 0;
    record->baseAngleY = 0;
    record->baseAngleZ = 0;
    record->offsetAngleX = 0;
    record->offsetAngleY = 0;
    record->offsetAngleZ = 0;
    record->flags = 0x8000;
    memset(record, 0, 8);
}

Ovl11SceneRecordSlot *Ovl11FindFreeSceneRecord(void);

/* Eight-byte transform chunks retain the source's unaligned-copy contract. */
typedef struct __attribute__((packed)) {
    u8 bytes[8];
} Ovl11SceneTransformChunk;

typedef struct {
    s32 flags;
    s32 meshOffset;
    s32 blendOffset1;
    s32 blendOffset2;
    u16 duration;
    u16 reserved12;
    Ovl11SceneTransformChunk transform[3];
} Ovl11SceneMapEntry;

void Ovl11InstallSceneRecords(void) {
    Ovl11SceneRecordSlot *record;
    Ovl11VertexBlend *blend;
    u8 *meshBase;
    s32 remaining;
    u16 flags;
    Ovl11SceneMapEntry *entry;
    u8 *work;

    remaining = 0xE;
    work = (u8 *)&D_8007AFF0;
    work += 0x20000;
    entry = (Ovl11SceneMapEntry *)(*(u8 **)(work + 0x5388) + 0x948);
    meshBase = (u8 *)D_8007AFF0 + 0x2000;
    do {
        if (!(entry->flags & 0x8000)) {
            record = Ovl11FindFreeSceneRecord();
            if (record != NULL) {
                *(Ovl11SceneTransformChunk *)&record->prefix[8] = entry->transform[0];
                *(Ovl11SceneTransformChunk *)&record->prefix[0x10] = entry->transform[1];
                *(Ovl11SceneTransformChunk *)&record->prefix[0x18] = entry->transform[2];
                InitProjectedPrimitiveSource(record, meshBase + entry->meshOffset);
                flags = (u16) entry->flags;
                record->flags = flags;
                if (flags & 0x4000) {
                    blend = Ovl11AllocVertexBlend(meshBase + entry->meshOffset, meshBase + entry->blendOffset1, meshBase + entry->blendOffset2, &record->prefix[8]);
                    (*(Ovl11VertexBlend **) ((u8 *) record + 0x2C)) = blend;
                    (*(s16 *) ((u8 *) blend + 0x2E)) = 0;
                    (*(u16 *) ((u8 *) (*(Ovl11VertexBlend **) ((u8 *) record + 0x2C)) + 0x30)) = entry->duration;
                }
            }
        }
        remaining -= 1;
        entry++;
    } while (remaining >= 0);
    Ovl11InitSceneAnimationCycle();
    Ovl11InitSceneAnimationPair();
    Ovl11InitSceneZRotationRamp();
    Ovl11InitSceneYRotationRamp();
    Ovl11InitSceneRecordVisibilityPair();
}
