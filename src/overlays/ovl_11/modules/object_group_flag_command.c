/* Group/record selection followed by flag or item-state commands.
 * Public contract: ovl11_object_group_flag_command.h.
 * Membership/boundaries: notes/file-groupings.md. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "game_flags.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_object_group_flag_command.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_stage_mode1_record.h"

s32 Ovl11ApplyObjectGroupCommand(s16 group, s16 recordIndex, s32 operation,
                                 s32 indexFromSlot) {
    s16 index;
    s32 i;
    s32 offset;
    s32 *flags;
    s32 *smallPoolFlags;
    char *work;
    char *smallPoolBase;
    struct_800759E4 *record;

    if (indexFromSlot != 0) {
        index = *(s16 *)&gOvl11ValueSlots[recordIndex];
    } else {
        index = recordIndex;
    }
    record = NULL;
    switch (group) {
    case 0:
        record = (struct_800759E4 *)&gStageMode2Record;
        break;
    case 1:
    case 2:
        if (operation == 0) {
            i = 0;
            work = (char *)D_8006C838;
            work += 0x81F0;
            flags = (s32 *)work;
            do {
                if (flags[i * 0x2E] & 0x400) {
                    offset = (s16)i;
                    record = (struct_800759E4 *)((char *)&gTaggedRecordBase + offset * 0xB8);
                    break;
                }
                i++;
            } while (i < 20);
            if (record == NULL) {
                return 1;
            }
        } else {
            record = (struct_800759E4 *)((char *)&gTaggedRecordBase + index * 0xB8);
        }
        break;
    case 3:
        if (operation == 0) {
            i = 0;
            smallPoolFlags = &D_80074320;
            for (; i < 10; i++) {
                if (smallPoolFlags[i * 0x2D] & 0x400) {
                    smallPoolBase = (char *)(smallPoolFlags - 0xD);
                    offset = (s16)i;
                    record = (struct_800759E4 *)(offset * 0xB4 + (s32)smallPoolBase);
                    break;
                }
            }
            if (record == NULL) {
                return 1;
            }
        } else {
            record = (struct_800759E4 *)((char *)&gSmallTaggedRecordBase + index * 0xB4);
        }
        break;
    case 4:
        record = &gStageMode1Record;
        break;
    default:
        return 0;
    }
    switch (operation) {
    case 0:
        record->f34 &= ~0x400;
        break;
    case 1:
        record->f34 |= 0x400;
        break;
    case 2:
        SetGameFlag(0x74, 0);
        Ovl11SetStageMode1PositionPreset(0);
        break;
    case 3:
        Ovl11MatchItemSlotsCommand(0x106, 1, 0, 1);
        ((struct Ovl11PoolResetView *)D_8006C838)->field_91E0 &= ~0x20000;
        SetGameFlag(0x74, 1);
        Ovl11SetStageMode1PositionPreset(1);
        break;
    }
    return 1;
}
