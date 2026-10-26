#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800BEF64(Ovl11CollisionQuery *arg0) {
    s32 box[6];
    s32 *bp;
    Ovl11ActorCollisionRecord *entry;
    s32 *position;
    s32 i;
    u16 flags;
    u16 originalFlags;

    bp = box;
    position = arg0->position;
    box[0] = position[0] - 0xC8;
    box[1] = position[0] + 0xC8;
    box[2] = position[2] - 0xC8;
    box[3] = position[2] + 0xC8;
    entry = D_800763F4;
    box[4] = position[1] - 0x14;
    box[5] = position[1] + 0x14;
    if (gPrimaryObjectFlags & 8) {
        return 0;
    }
    for (i = 1; i < 0x25; i++, entry++) {
        originalFlags = entry->field_1E;
        if (!(originalFlags & 0x1000)) {
            flags = originalFlags & 0xFFFB;
            entry->field_1E = flags;
            if (entry->field_2E == arg0->object->field_2 &&
                (flags & 0x100) && !(entry->field_1E & 3) &&
                !(entry->field_20 & 0x1000) &&
                bp[0] < entry->position[0] && entry->position[0] < bp[1] &&
                bp[2] < entry->position[2] && entry->position[2] < bp[3] &&
                bp[4] < entry->position[1] && entry->position[1] < bp[5]) {
                *arg0->flags = arg0->field_18;
                if (entry->field_A < 0x1E) {
                    *arg0->flags |= 0x4000;
                }
                *arg0->tile = entry;
                return 1;
            }
        } else if ((originalFlags & 0x800) &&
                   bp[0] < entry->alternatePosition[0] && entry->alternatePosition[0] < bp[1] &&
                   bp[2] < entry->alternatePosition[2] && entry->alternatePosition[2] < bp[3]) {
            *arg0->flags = arg0->field_18;
            *arg0->tile = entry;
            *arg0->flags |= 0x4000;
            return 1;
        }
    }
    return 0;
}
