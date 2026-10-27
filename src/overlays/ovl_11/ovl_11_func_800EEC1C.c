#include "common.h"
#include "game_types.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"
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
#include "ovl11_held_item_replacement.h"

s32 ovl_11_func_800EEC1C(s16 id, s16 which, s16 last, s32 amount) {
    s32 key;
    s32 end;
    s32 start;
    s32 group;
    s32 i;
    s32 count;
    s16 found;
    Ovl11ItemState *table;
    Ovl11ItemState *slot;

    key = id;
    end = last;
    table = 0;
    count = 0;
    found = 0;
    if (end == -1) {
        start = 1;
        end = 1;
    } else {
        start = 0;
    }
    for (group = start; group <= end; group++) {
        switch (group) {
        case 0:
            table = (Ovl11ItemState *)&D_80071A84 + which;
            count = 1;
            break;
        case 1:
            if (which == 0) {
                table = (Ovl11ItemState *)&gItemStackSlotBase;
            } else {
                table = (Ovl11ItemState *)&gPrimaryObjectSecondaryItemList;
            }
            count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
            break;
        case 2:
            if (which == 0) {
                table = (Ovl11ItemState *)&gItemOverflowSlotBase;
                count = 64;
            } else {
                table = &D_80070D42_slots[0][0];
                count = 64;
            }
            break;
        case 3:
            if (which == 0) {
                table = D_80071042;
                count = 64;
            } else {
                table = 0;
                count = 0;
            }
            break;
        }
        for (i = start; i < count; i++) {
            slot = table + i;
            if (slot->id == key) {
                if (amount != 0) {
                    switch (group) {
                    case 0:
                        if (which != 0) {
                            ((struct Ovl11ItemSlotSnapshotWork *)D_8006C838)->flags &= 0xEFFF7FFF;
                            Ovl11ResetPrimaryObjectRequest();
                            Ovl11ReleaseHeldItem((Ovl11C7010Object *)((u8 *)D_8006C838 + 0x51C8));
                        } else {
                            Ovl11RemoveSlotSprite(slot->id);
                            Ovl11ClearHalfwordTriplet((s16 *)slot);
                        }
                    case 1:
                        Ovl11RemoveSlotSprite(slot->id);
                        Ovl11ClearHalfwordTriplet((s16 *)slot);
                        break;
                    case 2:
                    case 3:
                        Ovl11ClearHalfwordTriplet((s16 *)slot);
                        break;
                    }
                    amount--;
                }
                found++;
            }
        }
    }
    return found;
}
