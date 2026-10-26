#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_moving_items.h"
#include "ovl11_primary_object_state_change.h"

s32 ovl_11_func_800CDD7C(Ovl11C7010Object *arg0) {
    u16 effect[3];

    arg0->unk6C |= 0x1870;
    Ovl11ChangePrimaryObjectState(arg0, 0x45, 0, -2);
    SelectSpriteAnimation((ObjectState *) &arg0->sprite, *(u8 *) ((u8 *) arg0 + 0x38));
    PlaySoundEffect(0xF);
    effect[0] = 0x186;
    effect[1] = 0;
    effect[2] = 1;
    Ovl11SpawnMovingItemRecord((Vec3 *) ((u8 *) arg0 + 0x100), effect, (s16) arg0->unk38);
    gOvl11HeldActionSpritePosition.x = 0;
    gOvl11HeldActionSpritePosition.y = 0;
    gOvl11HeldActionSpritePosition.z = 0;
    gOvl11HeldActionSpriteHeader = 0x1B;
    if (arg0->unk84 == 0x39) {
        gOvl11HeldActionSpriteHeader = 0x1C;
    }
    gOvl11HeldActionSpriteStep = 0;
    gOvl11HeldActionSignal = 0;
    return 1;
}
