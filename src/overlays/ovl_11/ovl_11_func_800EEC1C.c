#include "common.h"
#include "game_types.h"
#include "integer_power.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"

void ovl_11_func_800CBD78(Ovl11C7010Object *);
void ovl_11_func_800D666C(s16);

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
                table = (Ovl11ItemState *)&Ovl11Items_80071AC0;
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
                            ((struct Ovl11E8250State *)D_8006C838)->flags &= 0xEFFF7FFF;
                            Ovl11ResetPrimaryObjectRequest();
                            ovl_11_func_800CBD78((Ovl11C7010Object *)((u8 *)D_8006C838 + 0x51C8));
                        } else {
                            ovl_11_func_800D666C(slot->id);
                            Ovl11ClearHalfwordTriplet((s16 *)slot);
                        }
                    case 1:
                        ovl_11_func_800D666C(slot->id);
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
