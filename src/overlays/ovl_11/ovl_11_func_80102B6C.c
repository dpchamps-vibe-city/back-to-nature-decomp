#include "common.h"
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

void ovl_11_func_80102B6C(u16 arg0, s16 arg1) {
    s32 flags;

    Ovl11InstallSlotSprite(arg0);
    ((struct Ovl11SpriteRequestState *) D_8006C838)->id = arg0;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->value = arg1;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->active = 1;
    flags = ((struct Ovl11SpriteRequestState *) D_8006C838)->flags;
    flags |= 0x2000;
    ((struct Ovl11SpriteRequestState *) D_8006C838)->flags = flags;
}
