#include "common.h"

#include "game_types.h"
#include "ovl11_halfword_triplets.h"
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

void ovl_11_func_800CCF14(Ovl11TripletTailView *arg0) {
    if (Ovl11AddTripletTail(arg0, -1) != 0) {
        Ovl11RemoveSlotSprite(*(s16 *) ((u8 *) arg0 + 0));
        Ovl11ClearHalfwordTriplet((s16 *) arg0);
    }
}
