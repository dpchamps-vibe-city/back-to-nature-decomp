/* Seasonal resident-header binding for the shared slot-sprite source.
 * State is shared with the slot drawing services, not owned by this unit. */
#include "common.h"
#include "game_types.h"
#include "sprite_source.h"
#include "ovl11_slot_sprite_cache.h"
#include "ovl11_slot_sprite_source_refresh.h"

void Ovl11RefreshSharedSlotSpriteSource(void) {
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
