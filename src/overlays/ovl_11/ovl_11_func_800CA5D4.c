#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"


void ovl_11_func_800CA5D4(Ovl11C7010Object *arg0) {
    ObjectState *temp_s1;
    u8 *temp_s0;

    temp_s0 = Ovl11SavedInteractionObject;
    *(u16 *) (temp_s0 + 0x22) = arg0->unk38;
    if (arg0->unk3C == 1) {
        SelectSpriteAnimation((ObjectState *) ((u8 *) temp_s0 + 0x78), 0);
    }
    temp_s1 = (ObjectState *) ((u8 *) temp_s0 + 0x78);
    if (arg0->unk3C == 2) {
        SelectSpriteAnimation(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
    }
    if (arg0->unk3C == 4) {
        SelectSpriteAnimation(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
    }
    if (arg0->sprite.field_4 == (arg0->unk38 + 0x3F)) {
        SelectSpriteAnimation(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
        return;
    }
    if (GetSpriteFrameControlHighBit((SpriteSourceData *) temp_s1) != 0) {
        temp_s0[0x7D] = temp_s0[0x22];
    }
}
