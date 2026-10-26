#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_record_placement.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_primary_object_sprite_slots.h"

void ovl_11_func_800CBD78(Ovl11C7010Object *arg0) {
    VECTOR sp10;
    s16 temp_s0;

    temp_s0 = arg0->itemIndex;
    if (gItemDataTable[temp_s0].u0.flags & 0x100) {
        Ovl11ComputeGridOffsetPosition((Ovl11CE034Object *) arg0, (Ovl11CE034Position *) &sp10, 2);
        Ovl11RefreshRecordById(temp_s0, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), &sp10);
    }
    Ovl11RemoveSlotSprite(temp_s0);
    Ovl11ClearHalfwordTriplet(&arg0->itemIndex);
}
