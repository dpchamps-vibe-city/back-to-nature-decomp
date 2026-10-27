/* Enabled-state flags for initial record-pool partition 6
 * (ovl_11:800F4AC0–800F4B5C). The two services traverse the same sixteen
 * 0x18-byte records through work+0xDDA4; bit 0x4000 means disabled.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "ovl11_record_partition_flags.h"

void Ovl11SetPartition6Enabled(s32 enabled) {
    Ovl11DispatchRecord *record;
    char *upperWork;
    s32 index;
    u16 flags;

    upperWork = (char *)&D_8006C838;
    upperWork += 0x8000;
    record = *(Ovl11DispatchRecord **)(upperWork + 0x5DA4);
    for (index = 0; index < 16; index++) {
        if (enabled == 1) {
            flags = record->flags & 0xBFFF;
        } else {
            flags = record->flags | 0x4000;
        }
        record->flags = flags;
        record += 1;
    }
}

s32 Ovl11CountEnabledPartition6Records(void) {
    s32 count;
    Ovl11DispatchRecord *record;
    char *upperWork;
    s32 remaining;

    upperWork = (char *)&D_8006C838;
    upperWork += 0x8000;
    record = *(Ovl11DispatchRecord **)(upperWork + 0x5DA4);
    count = 0;
    remaining = 0xF;
    do {
        if (!(record->flags & 0x4000)) {
            count += 1;
        }
        record += 1;
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
