/* Start the held-item effect used for codes 38/39. This seeds the shared
 * sprite feedback consumed by moving_items and held_action_selection; it
 * does not own those services or allocate a sprite source. */
#include "common.h"
#include "game_types.h"
#include "ovl11_held_item_effect.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_moving_items.h"
#include "ovl11_primary_object_state_change.h"

s32 Ovl11BeginHeldItemEffect(Ovl11C7010Object *object) {
    u16 effect[3];

    object->unk6C |= 0x1870;
    Ovl11ChangePrimaryObjectState(object, 0x45, 0, -2);
    SelectSpriteAnimation((ObjectState *)&object->sprite, *(u8 *)((u8 *)object + 0x38));
    PlaySoundEffect(0xF);
    effect[0] = 0x186;
    effect[1] = 0;
    effect[2] = 1;
    Ovl11SpawnMovingItemRecord((Vec3 *)((u8 *)object + 0x100), effect, (s16)object->unk38);
    gOvl11HeldActionSpritePosition.x = 0;
    gOvl11HeldActionSpritePosition.y = 0;
    gOvl11HeldActionSpritePosition.z = 0;
    gOvl11HeldActionSpriteHeader = 0x1B;
    if (object->unk84 == 0x39) {
        gOvl11HeldActionSpriteHeader = 0x1C;
    }
    gOvl11HeldActionSpriteStep = 0;
    gOvl11HeldActionSignal = 0;
    return 1;
}
