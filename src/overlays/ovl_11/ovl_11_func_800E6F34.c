#include "common.h"
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

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_800E6F34(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 item[3];
    u16 id;
    s32 count;
    s16 i;
    Ovl11ItemState *slot;

    if (arg1 != 0) {
        id = *(u16 *)&gOvl11ValueSlots[arg0];
    } else {
        id = arg0;
    }
    count = pow_int(2, ((struct Ovl11ItemGridState *)D_8006C838)->exponent + 1);
    if (arg2 == 0) {
        if (!ovl_11_func_800E6EE0(0, 1, 0, 0)) {
            return 0;
        }
        Ovl11SetHalfwordTriplet(item, id, 0, 1);
        Ovl11InstallPrimaryObjectRequest((u16 *)item);
        return 1;
    } else {
        if (!ovl_11_func_800E6EE0(0, 1, -1, 0)) {
            return 0;
        }
        slot = ((struct Ovl11ItemGridState *)D_8006C838)->slots;
        for (i = 0; i < count && slot->id != 0; i++, slot++) {
        }
        Ovl11InstallSlotSprite(id);
        Ovl11SetHalfwordTriplet((s16 *)slot, id, 0, 1);
        return 1;
    }
}
