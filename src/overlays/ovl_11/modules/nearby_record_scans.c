/*
 * nearby_record_scans — reconstructed translation unit.
 *
 * Open-box scans around a signed-halfword position: active actor search and
 * region-filtered item identifier search.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_nearby_record_scans.h"

struct_80076220 *Ovl11FindNearbyActiveActor(s16 *position, void *unusedContext) {
    struct_80076220 *actor;
    s32 *bounds;
    s32 box[6];
    s32 *actorPosition;
    u32 slot;
    u16 activeMask;
    u8 *positionBytes;

    bounds = box;
    actor = &gObjectEventSlotBase;
    slot = 0;
    box[0] = position[0] - 0x96;
    box[1] = position[0] + 0x96;
    box[2] = position[2] - 0x96;
    box[3] = position[2] + 0x96;
    box[4] = position[1] - 0x14;
    box[5] = position[1] + 0x14;
    for (; slot < 0x25; slot++, actor++) {
        if (actor->unk1E & 0x1000) {
            activeMask = 0x800;
            positionBytes = &actor->unk30[0x30];
        } else {
            activeMask = 0x100;
            positionBytes = actor->unk30;
        }
        actorPosition = (s32 *)positionBytes;
        if ((actor->unk1E & activeMask) &&
            bounds[0] < actorPosition[0] && actorPosition[0] < bounds[1] &&
            bounds[2] < actorPosition[2] && actorPosition[2] < bounds[3] &&
            bounds[4] < actorPosition[1] && actorPosition[1] < bounds[5]) {
            return actor;
        }
    }
    return 0;
}

u16 Ovl11FindNearbyItemId(s16 *position, s16 *regionTag) {
    char *workBase;
    PoolRecord18 *item;
    s32 *bounds;
    s32 box[4];
    u32 slot;

    bounds = box;
    slot = 0;

    /* Retain the two-stage work-area address used by the original code. */
    workBase = (char *)&D_8006C838;
    workBase += 0x8000;
    item = *(PoolRecord18 **)(workBase + 0x5D8C);

    box[0] = position[0] - 0x96;
    box[1] = position[0] + 0x96;
    box[2] = position[2] - 0x96;
    box[3] = position[2] + 0x96;

    for (; slot < 0xC5; slot++) {
        if (regionTag[1] == item->unk0) {
            if (bounds[0] < item->unk8 && item->unk8 < bounds[1] &&
                bounds[2] < item->unk10 && item->unk10 < bounds[3]) {
                return item->unk2;
            }
        }
        item++;
    }
    return 0;
}
