/* Item-slot command family: shared operand matching, empty-slot checks and
 * insertion through primary request, grid or held/overflow paths. Records are
 * six-byte id/variant/count triplets. Storage belongs to the shared work area,
 * not this module. Interface: ovl11_item_slot_commands.h; unit evidence and
 * conservative -G0 boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_item_slot_commands.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_primary_object_grid_actions.h"
#include "psyq/memory.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"

s32 Ovl11HasEmptyItemSlot(s16 which, s16 lastGroup, s16 unused2, s16 unused3) {
    s32 found;

    found = Ovl11MatchItemSlotsCommand(0, which, lastGroup, 0);
    return found;
}

s32 ovl_11_func_800EEC1C(s16 id, s16 which, s16 last, s32 amount);

s32 Ovl11MatchItemSlotsCommand(s16 itemOrSlot, s16 which, s32 lastGroup, s32 flags) {
    s16 itemId;

    itemId = itemOrSlot;
    if (flags & 2) {
        itemId = *((s16 *)&gOvl11ValueSlots[itemId]);
    }
    return ovl_11_func_800EEC1C(itemId, which, lastGroup, flags & 1) != 0;
}

s32 Ovl11InsertSecondaryItemCommand(s16 itemOrSlot, s16 fromValueSlot,
                                   s32 useGrid, s32 unused) {
    s16 item[3];
    u16 itemId;
    s32 capacity;
    s16 index;
    Ovl11ItemState *slot;

    if (fromValueSlot != 0) {
        itemId = *(u16 *)&gOvl11ValueSlots[itemOrSlot];
    } else {
        itemId = itemOrSlot;
    }
    capacity = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
    if (useGrid == 0) {
        if (!Ovl11MatchItemSlotsCommand(0, 1, 0, 0)) {
            return 0;
        }
        Ovl11SetHalfwordTriplet(item, itemId, 0, 1);
        Ovl11InstallPrimaryObjectRequest((u16 *)item);
        return 1;
    } else {
        if (!Ovl11MatchItemSlotsCommand(0, 1, -1, 0)) {
            return 0;
        }
        slot = ((struct Ovl11ItemGridState *)D_8006C838)->slots;
        for (index = 0; index < capacity && slot->id != 0; index++, slot++) {
        }
        Ovl11InstallSlotSprite(itemId);
        Ovl11SetHalfwordTriplet((s16 *)slot, itemId, 0, 1);
        return 1;
    }
}

s32 Ovl11InsertHeldGridOrOverflowItem(s16 itemOrSlot, s16 fromValueSlot,
                                     s32 excludeOverflowCheck, s32 feedbackMode) {
    s16 itemId;
    s16 variant;
    s16 *selected;
    Ovl11ItemState *slot;
    s32 inOverflow;
    s32 gridIndex;
    s32 capacity;
    u32 index;
    s32 lastGroup;
    s32 result;

    lastGroup = 2;
    if (excludeOverflowCheck != 0) {
        lastGroup = 1;
    }
    if (!Ovl11MatchItemSlotsCommand(0, 0, lastGroup, 0)) {
        return 0;
    }
    if (fromValueSlot != 0) {
        itemId = *(s16 *)&gOvl11ValueSlots[itemOrSlot];
    } else {
        itemId = itemOrSlot;
    }
    selected = 0;
    inOverflow = 0;
    gridIndex = -1;
    if (((struct Ovl11ItemInsertState *)D_8006C838)->held.id == 0) {
        selected = (s16 *)&((struct Ovl11ItemInsertState *)D_8006C838)->held;
    } else {
        capacity = pow_int(2, ((struct Ovl11ItemInsertState *)D_8006C838)->exponent + 1);
        slot = ((struct Ovl11ItemInsertState *)D_8006C838)->slots;
        for (index = 0, gridIndex = 0; (s32)index < capacity; index++, slot++, gridIndex++) {
            if (slot->id == 0) {
                selected = (s16 *)slot;
                break;
            }
        }
        if (selected == 0) {
            slot = (Ovl11ItemState *)&gItemOverflowSlotBase;
            for (index = 0; index < 64; index++, slot++) {
                if (slot->id == 0) {
                    selected = (s16 *)slot;
                    inOverflow = 1;
                    break;
                }
            }
        }
    }
    result = 0;
    if (selected != 0) {
        variant = 0;
        if ((u16)(itemId - 1) < 25U) {
            variant = (itemId - 1) % 5;
        }
        if (inOverflow == 0) {
            Ovl11InstallSlotSprite(itemId);
        }
        Ovl11SetHalfwordTriplet(selected, itemId, variant, 1);
        if (inOverflow == 0 && feedbackMode != 0) {
            Ovl11BeginSelectedRecordTransfer((Recon_ovl_11_func_800C7270_A0View *)&gPrimaryObjectBase,
                               gridIndex != -1, gridIndex, feedbackMode == 1 ? 10 : 60);
        }
        result = 1;
    }
    return result;
}
