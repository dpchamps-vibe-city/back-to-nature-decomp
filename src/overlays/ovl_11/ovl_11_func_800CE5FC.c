#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "psyq/memory.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/libsnd.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"
#include "ovl11_slot_sprite_cache.h"

extern s32 gOvl11SharedSlotSpriteEnabled;
extern s16 gCalendarSeason;

void ovl_11_func_800CE5FC(void) {
    s32 header;

    gOvl11SharedSlotSpriteEnabled = 0;
    if (gCalendarSeason == 3) {
        header = Ovl11FindResidentSlotSprite(0x187);
        if (header != 0) {
            gOvl11SharedSlotSpriteEnabled = 1;
            InitSpriteSourceData(&gOvl11SharedSlotSpriteSource, (SpriteDataHeader *)header);
        }
    }
}
